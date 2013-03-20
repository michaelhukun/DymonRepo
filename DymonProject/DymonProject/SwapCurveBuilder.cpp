//created by Wang Jianwei on 1 Dec 2012
//Added cashflowleg creating and swap section - Kun
#include "SwapCurveBuilder.h"
#include <iostream>
#include "DiscountCurve.h"
#include "CashFlowLegBuilder.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "EnumHelper.h"
#include "dateUtil.h"
#include "RecordHelper.h"
#include "DepositRateBootStrapper.h"
#include "OvernightRateBootStrapper.h"
#include "SwapRateBootStrapper.h"
#include "AbstractBootStrapper.h"
#include "Constants.h"
#include "Swap.h"

using namespace utilities;
typedef AbstractBuilder super;
typedef tuple<date, double> point;

void SwapCurveBuilder::init(Configuration* cfg){
	super::init(cfg);
	_timeLineBuildDirection = std::stoi(cfg->getProperty("SwapDiscountCurve.buildCashFlowDirection",false,"1"));
	_rollAccuralDates =  cfg->getProperty("SwapDiscountCurve.rollAccuralDates",false,"0")=="0"?false:true;
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("SwapDiscountCurve.interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("SwapDiscountCurve.numerical",false,"BISECTION"));
	_interpolRateType = EnumHelper::getRateType(cfg->getProperty("SwapDiscountCurve.interpol.rate",true,"ZERO"));
	_spotDateDF = NaN;
}

DiscountCurve* SwapCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	DiscountCurve* yc = new DiscountCurve();
	yc->setName(_market.getNameString()+" Swap Curve");
	yc->setDayCount(ACT_365);
	yc->setInterpolRateType(_interpolRateType);

	loadRateMaps();
	buildDepositSection(yc);
	buildSwapSection(yc);
	return yc;
}

void SwapCurveBuilder::loadRateMaps(){
	if (_market.getShortEndUseLibor()){
		_shortEndMap  = RecordHelper::getInstance()->getLiborRateMap()->at(_market.getCurrencyEnum());
	}else{
		_shortEndMap = RecordHelper::getInstance()->getDepositRateMap()->at(_market.getCurrencyEnum());
	}
	_longEndMap = RecordHelper::getInstance()->getSwapRateMap()->at(_market.getCurrencyEnum());
}

void SwapCurveBuilder::buildDepositSection(DiscountCurve* yc){			
	
	for (auto it=_shortEndMap.begin(); it != _shortEndMap.end(); it++ ){
		date accrualEndDate = ((*it).first);
		Deposit* deposit = &(it->second);

		if (it==_shortEndMap.begin()){
			_curveStartDate = deposit->getTradeDate();
			_curvePointer = point(_curveStartDate,1);
			_spotDate = dateUtil::getBizDateOffSet(_curveStartDate,_market.getBusinessDaysAfterSpotSwap(),_market.getCurrencyEnum());
		}

		AbstractBootStrapper<date>* bs;
		if (deposit->getIsOverNight()){
			bs = new OvernightRateBootStrapper(_curvePointer, deposit->getDeliveryDate(), deposit, yc, _interpolAlgo, _numericalAlgo);
		}else{
			bs = new DepositRateBootStrapper(_curvePointer, deposit->getDeliveryDate(), deposit, yc, _interpolAlgo, _numericalAlgo, _spotDateDF);
		}
		bs->setSpotDate(_spotDate);
		bs->init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = bs->bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();

		if (_spotDateDF == NaN && _spotDate<= std::get<0>(lineSection->getEndPoint())) 
			_spotDateDF = yc->getValue(_spotDate);
	}
}

void SwapCurveBuilder::buildSwapSection(DiscountCurve* yc){
	for (auto it=_longEndMap.begin(); it != _longEndMap.end(); it++ ){

		date accrualEndDate=it->first;	
		Swap* swap=&(it->second);		
		date paymentDate = swap->getMaxSwapAndResetDeliveryDate();

		//cout << "Swap rate at fixing date ["<<fixingDate.toString()<<"], accrual end date ["<<accrualEndDate.toString()<<"], payment day ["<<paymentDate.toString()<<"], rate ["<< swapRate<<"]"<< endl;

		SwapRateBootStrapper swapBS(_curvePointer, paymentDate, swap, yc, _interpolAlgo,_numericalAlgo);
		swapBS.setSpotDate(_spotDate);
		swapBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = swapBS.bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer=lineSection->getEndPoint();
	}
}