//created by Wang Jianwei on 1 Dec 2012
//Added cashflowleg creating and swap section - Kun
#include "DiscountCurveBuilder.h"
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

using namespace utilities;
typedef AbstractBuilder super;
typedef tuple<date, double> point;

void DiscountCurveBuilder::init(Configuration* cfg){
	super::init(cfg);
	
	_market = Market(EnumHelper::getCcyEnum("USD"));
	_curveStartDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,_market.getMarketEnum());
	_floatFreqency = std::stoi(cfg->getProperty("swap.usd.floatfreq",false,"4"));
	_fixFreqency = std::stoi(cfg->getProperty("swap.usd.fixfreq",false,"2"));
	_timeLineBuildDirection = std::stoi(cfg->getProperty("timeline.usd.builddirection",false,"1"));
	_rollAccuralDates =  cfg->getProperty("timeline.usd.rollaccuraldates",false,"0")=="0"?false:true;
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("DiscountCurve.usd.interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("DiscountCurve.usd.numerical",false,"BISECTION"));
	_bizDaysAfterSpot = _market.getBusinessDaysAfterSpot();
	_bizDaysAfterSpotDF = NaN;
}

DiscountCurve* DiscountCurveBuilder::build(){
	DiscountCurve* yc = new DiscountCurve();
	buildOvernightSection(yc);
	buildDepositSection(yc);
	buildSwapSection(yc);
	return yc;
}

void DiscountCurveBuilder::buildOvernightSection(DiscountCurve* yc){
	point lineStartPoint(_curveStartDate,1);
	_curvePointer = lineStartPoint;
	map<long,double> rateMap = RecordHelper::getInstance()->getOverNightRateMap()[enums::USD];
	for (map<long,double>::iterator it=rateMap.begin(); it != rateMap.end(); it++ ){
		double depositRate = (*it).second;
		date startDate = _curveStartDate;
		int numOfNights = (int) (*it).first;
		date paymentDate = dateUtil::getBizDateOffSet(startDate,numOfNights,_market.getMarketEnum());
		//cout << "Overnight rate at date ["<<startDate.toString()<< "], maturity date ["<<paymentDate.toString()<<"], number of nights ["<<numOfNights<<"], rate ["<< depositRate<<"]"<< endl;

		cashflow cf(depositRate,0, startDate, paymentDate,startDate, paymentDate, _market);
		OvernightRateBootStrapper overnightBS(_curvePointer, paymentDate, cf, _interpolAlgo, _numericalAlgo, _market);
		overnightBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = overnightBS.bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();

		if (numOfNights == _bizDaysAfterSpot) _bizDaysAfterSpotDF = std::get<1>(lineSection->getEndPoint());
	}
}

void DiscountCurveBuilder::buildDepositSection(DiscountCurve* yc){
	date accrualStartDate = dateUtil::getBizDateOffSet(_curveStartDate,_market.getBusinessDaysAfterSpot(),enums::USD);
	map<long,double> rateMap = RecordHelper::getInstance()->getDepositRateMap()[enums::USD];

	for (map<long,double>::iterator it=rateMap.begin(); it != rateMap.end(); it++ ){

		date fixingDate = _curveStartDate;
		date accrualEndDate((*it).first);
		date paymentDate = dateUtil::dayRollAdjust(accrualEndDate,_market.getDayRollCashConvention(),enums::USD);
		double depositRate = (*it).second;
		//cout << "Deposit rate at fixing date ["<<fixingDate.toString()<<"], accrual start date ["<<accrualStartDate.toString()<<"], accrual end date ["<<accrualEndDate.toString()<<"], payment day ["<<paymentDate.toString()<<"], rate ["<< depositRate<<"]"<< endl;

		cashflow cf(depositRate,0, fixingDate, paymentDate,accrualStartDate, accrualEndDate, _market);

		AbstractInterpolator<date>* lineSection;
		DepositRateBootStrapper depositBS(_curvePointer, paymentDate, cf, _interpolAlgo, _numericalAlgo, _market, _bizDaysAfterSpotDF);
		depositBS.init(Configuration::getInstance());
		lineSection = depositBS.bootStrap();
		yc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();

		if (_bizDaysAfterSpotDF == NaN) _bizDaysAfterSpotDF = std::get<1>(lineSection->getEndPoint());
	}
}

void DiscountCurveBuilder::buildSwapSection(DiscountCurve* yc){
	BuilderCashFlowLeg builtCashflowLeg(_curveStartDate,600,1,1, _floatFreqency, enums::USD);
	cashflowLeg* _cashflowLeg=builtCashflowLeg.getCashFlowLeg();
	//_cashflowLeg.printTimeLine();

	map<long,double> rateMap = RecordHelper::getInstance()->getSwapRateMap()[enums::USD];

	for (map<long,double>::iterator it=rateMap.begin(); it != rateMap.end(); it++ ){

		date fixingDate = _curveStartDate;
		date accrualEndDate=((*it).first);	
		double swapRate=(*it).second;
		date paymentDate = dateUtil::dayRollAdjust(accrualEndDate,_market.getDayRollSwapConvention(),enums::USD);

		//cout << "Swap rate at fixing date ["<<fixingDate.toString()<<"], accrual end date ["<<accrualEndDate.toString()<<"], payment day ["<<paymentDate.toString()<<"], rate ["<< swapRate<<"]"<< endl;

		AbstractInterpolator<date>* lineSection;
		SwapRateBootStrapper swapBS(_curvePointer, paymentDate, swapRate, _cashflowLeg, yc, _interpolAlgo,_numericalAlgo, _market.getDayCountSwapConvention());
		swapBS.init(Configuration::getInstance());
		lineSection = swapBS.bootStrap();
		yc->insertLineSection(lineSection);

		_curvePointer=lineSection->getEndPoint();

	}
}