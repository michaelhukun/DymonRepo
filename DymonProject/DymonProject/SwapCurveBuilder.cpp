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
#include "EuroDollarFutureBootStrapper.h"
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
	_swapCurveDayCount = EnumHelper::getDayCountEnum(cfg->getProperty("SwapDiscountCurve.daycount",false,"ACT/365"));
	_numberFuture = std::stoi(cfg->getProperty("SwapDiscountCurve.numberFutureUsed",false,"4"));
	_futureDaysBeforeExpiry = std::stoi(cfg->getProperty("SwapDiscountCurve.futureDaysBeforeExpiry",false,"14"));
	_spotDateDF = NaN;
}

DiscountCurve* SwapCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	DiscountCurve* yc = new DiscountCurve();
	yc->setName(_market.getNameString()+" Swap Curve");
	yc->setDayCount(_swapCurveDayCount);
	yc->setInterpolRateType(_interpolRateType);

	loadRateMaps();
	buildDepositSection(yc);
	buildEuroDollarFutureSection(yc);
	buildSwapSection(yc);
	yc->dumpComponentNames();
	return yc;
}

void SwapCurveBuilder::loadRateMaps(){
	if (_market.getShortEndUseLibor()){
		_shortEndMap  = RecordHelper::getInstance()->getLiborRateMap()->at(_market.getCurrencyEnum());
	}else{
		_shortEndMap = RecordHelper::getInstance()->getDepositRateMap()->at(_market.getCurrencyEnum());
	}
	_longEndMap = RecordHelper::getInstance()->getSwapRateMap()->at(_market.getCurrencyEnum());
	_midEndMap = RecordHelper::getInstance()->getEuroDollarFutureMap()->at(_market.getCurrencyEnum());
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
		yc->insertComponent(deposit);
		_curvePointer = lineSection->getEndPoint();

		if (_spotDateDF == NaN && _spotDate<= std::get<0>(lineSection->getEndPoint())) 
			_spotDateDF = yc->getValue(_spotDate);
	}
}

void SwapCurveBuilder::buildEuroDollarFutureSection(DiscountCurve* yc){
	int numberFutureUsed = 0;

	for (auto it=_midEndMap.begin(); it != _midEndMap.end(); it++ ){
		date accrualEndDate=it->first;	
		EuroDollarFuture* future=&(it->second);		
		if (!isFutureEligible(future, yc) || numberFutureUsed>=_numberFuture)
			return;

		removeLineSectionBeforeDate(yc, accrualEndDate);
		date paymentDate = future->getMaxFutureAndResetDeliveryDate();
		EuroDollarFutureBootStrapper futureBS(_curvePointer, paymentDate, future, yc, _interpolAlgo,_numericalAlgo);
		futureBS.setSpotDate(_spotDate);
		futureBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = futureBS.bootStrap();
		yc->insertLineSection(lineSection);
		yc->insertComponent(future);
		_curvePointer=lineSection->getEndPoint();
		numberFutureUsed++;
	}
}

void SwapCurveBuilder::buildSwapSection(DiscountCurve* yc){
	for (auto it=_longEndMap.begin(); it != _longEndMap.end(); it++ ){

		date accrualEndDate=it->first;	
		Swap* swap=&(it->second);		
		date paymentDate = swap->getMaxSwapAndResetDeliveryDate();
		if (get<0>(_curvePointer)>paymentDate)
			continue;

		SwapRateBootStrapper swapBS(_curvePointer, paymentDate, swap, yc, _interpolAlgo,_numericalAlgo);
		swapBS.setSpotDate(_spotDate);
		swapBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = swapBS.bootStrap();
		yc->insertLineSection(lineSection);
		yc->insertComponent(swap);
		_curvePointer=lineSection->getEndPoint();
	}
}

bool SwapCurveBuilder::isFutureEligible(EuroDollarFuture* future, DiscountCurve* yc){
	date curveStartDate = std::get<0>(yc->getCurveStartPoint());
	if (future->getExpiryDate()-curveStartDate>_futureDaysBeforeExpiry)
		return true;
	return false;
}

void SwapCurveBuilder::removeLineSectionBeforeDate(DiscountCurve* yc, date interestEndDate){
	for (int i=yc->getSize()-1; i>=0; i--){
		AbstractInterpolator<date>* lineSection = yc->getSection(i);
		date lineEndDate = get<0>(lineSection->getEndPoint());
		if (lineEndDate>=interestEndDate){
			yc->removeSection(i);
		_curvePointer = yc->getCurveEndPoint();
		}
		else
			return;
	}
}