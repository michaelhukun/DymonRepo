//created by Wang Jianwei on 1 Dec 2012
//minor modifications for numericalFunc on 19 Dec - Kun for accuralFactor

#include "SwapRateBootStrapper.h"
#include <iostream>
#include "DiscountCurve.h"
#include "InterpolatorFactory.h"
#include "AbstractInterpolator.h"
#include "NumericalFactory.h"
#include "dateUtil.h"
#include "EnumHelper.h"
#include <cmath>

using namespace utilities;

void SwapRateBootStrapper::init(Configuration* cfg){
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration",true,"50"));
	_plusMinus = std::stoi(cfg->getProperty("numerical.plusminus",true,"20"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance",true,"0.0000001"));
	_rateInterpolated = EnumHelper::getRateType(cfg->getProperty("SwapDiscountCurve.interpol.rate",true,"ZERO"));
}

AbstractInterpolator<date>* SwapRateBootStrapper::bootStrap(){

	_cashflowStartIndex = findCashFlowIndex(std::get<0>(_startPoint));
	_cashflowEndIndex = findCashFlowIndex(_endDate);

	AbstractNumerical<SwapRateBootStrapper>* an = NumericalFactory<SwapRateBootStrapper>::getInstance()->getNumerical(this,&SwapRateBootStrapper::numericalFunc,_numericAlgo);
	double previousVal = std::get<1>(_startPoint);
	double lowerBound = abs(previousVal*(1-_plusMinus/100.0));
	double upperBound = previousVal*(1+_plusMinus/100.0);
	double swapPointValue = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);

	AbstractInterpolator<date>* ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,swapPointValue) , _interpolAlgo);
	return ai;
}

double SwapRateBootStrapper::numericalFunc(double x){

	double numerator = _curve->getValue(_spotDate) - x;
	double denominator = 0;
	_interpolant = getInterpolant(_rateInterpolated, x);

	for( int i=0; i<=_cashflowStartIndex; i++){
		cashflow cf = _cashflowVector[i];
		double ithAccuralFactor=dateUtil::getAccrualFactor(cf.getAccuralStartDate(),cf.getAccuralEndDate(),cf.getDayCount());
		double ithDF = _curve->getDiscountFactor(cf.getPaymentDate());
		denominator = denominator + ithAccuralFactor*ithDF;
	}

	for( int i=_cashflowStartIndex+1; i<=_cashflowEndIndex; i++){
		cashflow cf = _cashflowVector[i]; 
		double ithAccuralFactor=dateUtil::getAccrualFactor(cf.getAccuralStartDate(),cf.getAccuralEndDate(),cf.getDayCount());
		double ithDF = getRateFromInterpolant(_rateInterpolated, cf.getPaymentDate());
		denominator = denominator + ithAccuralFactor*ithDF;
	}

	return numerator - _swap->getSwapRate()*denominator;
}

int SwapRateBootStrapper::findCashFlowIndex(date date0){
	for(unsigned int i = 0; i < _cashflowVector.size(); i++)
	{
		cashflow cf = _cashflowVector[i];
		if ( cf.getPaymentDate().getJudianDayNumber() == date0.getJudianDayNumber()){
			return i;
		}else if ( cf.getPaymentDate().getJudianDayNumber() > date0.getJudianDayNumber()){
			return i-1;
		}
	}
	return -1;
}

AbstractInterpolator<date>* SwapRateBootStrapper::getInterpolant(RateType rateType, double x){
	switch(rateType){
	case DF:
		return InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);
	case ZERO:
		date startDate = std::get<0>(_startPoint);
		double startPointZeroRate = _curve->getZeroRate(startDate);
		double endPointZeroRate =  _curve->getZeroRate(_endDate, x);
		point startPoint(startDate,startPointZeroRate);
		point endPoint(_endDate,endPointZeroRate);
		return InterpolatorFactory<date>::getInstance()->getInterpolator(startPoint, endPoint, _interpolAlgo);
	}
	throw "Interpolant not defined for this rate type. ";
}

double SwapRateBootStrapper::getRateFromInterpolant(RateType rateType, date paymentDate){
	switch(rateType){
	case DF:
		return std::get<1>(_interpolant->interpolate(paymentDate));
	case ZERO:
		double zeroRate = std::get<1>(_interpolant->interpolate(paymentDate));
		double discountFactor = _curve->getDiscountFactor(paymentDate, zeroRate);
		return discountFactor;
	}
	throw "Interpolant not defined for this rate type. ";
}