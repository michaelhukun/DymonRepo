//created by Wang Jianwei on 1 Dec 2012
//Added cashflowleg creating and swap section - Kun
#include "SwapCurveBuilder.h"
#include <iostream>
#include "DiscountCurve.h"
#include "BuilderCashFlowLeg.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "EnumHelper.h"
#include "dateUtil.h"
#include "RecordHelper.h"
#include "DepositRateBootStrapper.h"
#include "OvernightRateBootStrapper.h"
#include "SwapRateBootStrapper.h"
#include "Constants.h"
#include "Swap.h"

using namespace utilities;
typedef AbstractBuilder super;
typedef tuple<date, double> point;

void SwapCurveBuilder::init(Configuration* cfg){
	super::init(cfg);
	_curveStartDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,_market.getCurrencyEnum());
	_floatFreqency = std::stoi(cfg->getProperty("convention."+_market.getNameString()+".swap.floatfreq",false,"4"));
	_fixFreqency = std::stoi(cfg->getProperty("convention."+_market.getNameString()+".swap.fixfreq",false,"2"));
	_timeLineBuildDirection = std::stoi(cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".buildCashFlowDirection",false,"1"));
	_rollAccuralDates =  cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".rollAccuralDates",false,"0")=="0"?false:true;
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("SwapDiscountCurve."+_market.getNameString()+".numerical",false,"BISECTION"));
	_bizDaysAfterSpot = _market.getBusinessDaysAfterSpot(enums::SWAP);
	_bizDaysAfterSpotDF = NaN;
}

DiscountCurve* SwapCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	DiscountCurve* yc = new DiscountCurve();
	buildOvernightSection(yc);
	buildDepositSection(yc);
	buildSwapSection(yc);
	return yc;
}

void SwapCurveBuilder::buildOvernightSection(DiscountCurve* yc){
	point lineStartPoint(_curveStartDate,1);
	_curvePointer = lineStartPoint;
	auto rateMap = RecordHelper::getInstance()->getDepositRateMap()->at(_market.getCurrencyEnum());
	for (auto it=rateMap.begin(); it != rateMap.end(); it++ ){
		date accrualEndDate = ((*it).first);
		Deposit* deposit = &(it->second);
		date startDate = _curveStartDate;
		date paymentDate = deposit->getDeliveryDate();
		double depositRate = deposit->getDepositRate();
		//cout << "Overnight rate at date ["<<startDate.toString()<< "], maturity date ["<<paymentDate.toString()<<"], number of nights ["<<numOfNights<<"], rate ["<< depositRate<<"]"<< endl;

		cashflow cf(depositRate, 0, startDate, paymentDate,startDate, paymentDate, _market, true);
		OvernightRateBootStrapper overnightBS(_curvePointer, paymentDate, cf, _interpolAlgo, _numericalAlgo, _market);
		overnightBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = overnightBS.bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();

		if (2 == _bizDaysAfterSpot) _bizDaysAfterSpotDF = std::get<1>(lineSection->getEndPoint());
	}
}

void SwapCurveBuilder::buildDepositSection(DiscountCurve* yc){
	date accrualStartDate = dateUtil::getBizDateOffSet(_curveStartDate,_market.getBusinessDaysAfterSpot(enums::SWAP),_market.getCurrencyEnum());
	auto rateMap = RecordHelper::getInstance()->getDepositRateMap()->at(_market.getCurrencyEnum());

	for (auto it=rateMap.begin(); it != rateMap.end(); it++ ){

		date fixingDate = _curveStartDate;
		date accrualEndDate = ((*it).first);
		date paymentDate = dateUtil::dayRollAdjust(accrualEndDate,_market.getDayRollCashConvention(),_market.getCurrencyEnum());
		Deposit* deposit = &(it->second);
		double depositRate = deposit->getDepositRate();
		//cout << "Deposit rate at fixing date ["<<fixingDate.toString()<<"], accrual start date ["<<accrualStartDate.toString()<<"], accrual end date ["<<accrualEndDate.toString()<<"], payment day ["<<paymentDate.toString()<<"], rate ["<< depositRate<<"]"<< endl;

		cashflow cf(depositRate,0, fixingDate, paymentDate,accrualStartDate, accrualEndDate, _market, true);

		DepositRateBootStrapper depositBS(_curvePointer, paymentDate, cf, _interpolAlgo, _numericalAlgo, _market, _bizDaysAfterSpotDF);
		depositBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = depositBS.bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();

		if (_bizDaysAfterSpotDF == NaN) _bizDaysAfterSpotDF = std::get<1>(lineSection->getEndPoint());
	}
}

void SwapCurveBuilder::buildSwapSection(DiscountCurve* yc){
	BuilderCashFlowLeg builtCashflowLeg(enums::SWAP,_curveStartDate,600,1,1, _floatFreqency, _market.getCurrencyEnum());
	cashflowLeg* _cashflowLeg=builtCashflowLeg.getCashFlowLeg();

	auto swapRateMap = RecordHelper::getInstance()->getSwapRateMap()->at(_market.getCurrencyEnum());

	for (auto it=swapRateMap.begin(); it != swapRateMap.end(); it++ ){

		date fixingDate = _curveStartDate;
		date accrualEndDate=((*it).first);	
		Swap* swap=&(it->second);		
		date paymentDate = swap->getDeliveryDate();
		double swapRate = swap->getSwapRate();

		//cout << "Swap rate at fixing date ["<<fixingDate.toString()<<"], accrual end date ["<<accrualEndDate.toString()<<"], payment day ["<<paymentDate.toString()<<"], rate ["<< swapRate<<"]"<< endl;

		AbstractInterpolator<date>* lineSection;
		SwapRateBootStrapper swapBS(_curvePointer, paymentDate, swapRate, _cashflowLeg, yc, _interpolAlgo,_numericalAlgo, _market.getDayCountSwapConvention());
		swapBS.init(Configuration::getInstance());
		lineSection = swapBS.bootStrap();
		yc->insertLineSection(lineSection);

		_curvePointer=lineSection->getEndPoint();

	}
}