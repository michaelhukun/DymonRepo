//created by Wang Jianwei on 1 Dec 2012

#include "DepositRateBootStrapper.h"
#include <iostream>
#include <cmath>
#include "DiscountCurve.h"
#include "InterpolatorFactory.h"
#include "AbstractInterpolator.h"
#include "NumericalFactory.h"
#include "AbstractBootStrapper.h"
#include "dateUtil.h"
#include "RecordHelper.h"
#include "Constants.h"

using namespace utilities;

typedef AbstractBootStrapper<date> super;

void DepositRateBootStrapper::init(Configuration* cfg){
	super::init(cfg);
}

AbstractInterpolator<date>* DepositRateBootStrapper::bootStrap(){
	AbstractInterpolator<date>* ai;
	enums::DayCountEnum dayCountCashConvention = _market.getDayCountCashConvention();
	if (_spotDateDF != NaN){
		double accrualFactor = dateUtil::getAccrualFactor(_cashFlow.getAccuralStartDate(),_cashFlow.getAccuralEndDate(), dayCountCashConvention);
		double discountFactor = (1/(1+accrualFactor*_depositRate))*_spotDateDF;
		ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	}else{
		AbstractNumerical<DepositRateBootStrapper>* an = NumericalFactory<DepositRateBootStrapper>::getInstance()->getNumerical(this,&DepositRateBootStrapper::numericalFunc,_numericAlgo);
		double previousVal = std::get<1>(_startPoint);
		double lowerBound = abs(previousVal*(1-_plusMinus/100.0));
		double upperBound = previousVal*(1+_plusMinus/100.0);
		double discountFactor = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
		ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	}
	return ai;
}

double DepositRateBootStrapper::numericalFunc(double x){
	AbstractInterpolator<date>* ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);

	date spotDate = _cashFlow.getAccuralStartDate();
	double accrualFactor = dateUtil::getAccrualFactor(spotDate, _cashFlow.getAccuralEndDate(), _dayCountCash);

	double spotDateDF = std::get<1>(ai->interpolate(spotDate));
	double paymentDateDF = std::get<1>(ai->interpolate(_cashFlow.getPaymentDate()));

	double shouldBeZero = (1/(1+_depositRate*accrualFactor))*spotDateDF - paymentDateDF;
	return shouldBeZero;
}