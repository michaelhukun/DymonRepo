//created by Wang Jianwei on 1 Dec 2012
//minor modifications for numericalFunc on 19 Dec - Kun for accuralFactor

#include "SwapRateBootStrapper.h"
#include <iostream>
#include "DiscountCurve.h"
#include "InterpolatorFactory.h"
#include "AbstractInterpolator.h"
#include "NumericalFactory.h"
#include "dateUtil.h"
#include <cmath>

using namespace utilities;

void SwapRateBootStrapper::init(Configuration* cfg){
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration",true,"50"));
	_plusMinus = std::stoi(cfg->getProperty("numerical.plusminus",true,"20"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance",true,"0.0000001"));
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
	
	AbstractInterpolator<date>* ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);

	double numerator = _curve->getValue(_spotDate) - x;
	double denominator = 0;

	for( int i=0; i<=_cashflowStartIndex; i++){
		cashflow cf = _cashflowVector[i];
		double ithAccuralFactor=dateUtil::getAccrualFactor(cf.getAccuralStartDate(),cf.getAccuralEndDate(),cf.getDayCount());
		double ithDF = _curve->getDiscountFactor(cf.getPaymentDate());
		denominator = denominator + ithAccuralFactor*ithDF;
	}
	for( int i=_cashflowStartIndex+1; i<=_cashflowEndIndex; i++){
		cashflow cf = _cashflowVector[i]; 
		double ithAccuralFactor=dateUtil::getAccrualFactor(cf.getAccuralStartDate(),cf.getAccuralEndDate(),cf.getDayCount());
		double ithDF = std::get<1>(ai->interpolate(cf.getPaymentDate()));
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