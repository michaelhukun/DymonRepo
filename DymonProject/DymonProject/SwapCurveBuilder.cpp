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
	_timeLineBuildDirection = std::stoi(cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".buildCashFlowDirection",false,"1"));
	_rollAccuralDates =  cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".rollAccuralDates",false,"0")=="0"?false:true;
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".numerical",false,"BISECTION"));
	_spotDateDF = NaN;
}

DiscountCurve* SwapCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	DiscountCurve* yc = new DiscountCurve();
   yc->setName(_market.getNameString()+" Swap Curve");
	buildDepositSection(yc);
	buildSwapSection(yc);
	return yc;
}

void SwapCurveBuilder::buildDepositSection(DiscountCurve* yc){			
	auto rateMap = RecordHelper::getInstance()->getDepositRateMap()->at(_market.getCurrencyEnum());
	for (auto it=rateMap.begin(); it != rateMap.end(); it++ ){
		date accrualEndDate = ((*it).first);
		Deposit* deposit = &(it->second);

		if (it==rateMap.begin()){
			_curveStartDate = deposit->getTradeDate();
			_curvePointer = point(_curveStartDate,1);
		}

		cashflow cf(deposit, true);
		AbstractBootStrapper<date>* bs;
		if (deposit->getIsOverNight()){
			bs = new OvernightRateBootStrapper(_curvePointer, deposit->getDeliveryDate(), deposit, _interpolAlgo, _numericalAlgo);
		}else{
			_spotDate = deposit->getSpotDate();
			bs = new DepositRateBootStrapper(_curvePointer, deposit->getDeliveryDate(), cf, _interpolAlgo, _numericalAlgo, _market, _spotDateDF);
		}
		bs->init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = bs->bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();
		
		if (_spotDateDF == NaN && _spotDate<= std::get<0>(lineSection->getEndPoint())) 
			_spotDateDF = yc->getValue(_spotDate);
	}
}

void SwapCurveBuilder::buildSwapSection(DiscountCurve* yc){
	auto swapRateMap = RecordHelper::getInstance()->getSwapRateMap()->at(_market.getCurrencyEnum());
	for (auto it=swapRateMap.begin(); it != swapRateMap.end(); it++ ){

		date accrualEndDate=it->first;	
		Swap* swap=&(it->second);		
		date paymentDate = swap->getDeliveryDate();

		//cout << "Swap rate at fixing date ["<<fixingDate.toString()<<"], accrual end date ["<<accrualEndDate.toString()<<"], payment day ["<<paymentDate.toString()<<"], rate ["<< swapRate<<"]"<< endl;

		SwapRateBootStrapper swapBS(_curvePointer, paymentDate, swap, yc, _interpolAlgo,_numericalAlgo);
		swapBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = swapBS.bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer=lineSection->getEndPoint();
	}
}