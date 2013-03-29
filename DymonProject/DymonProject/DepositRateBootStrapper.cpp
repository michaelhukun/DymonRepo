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
	if (_spotDateDF != NaN){
		double accrualFactor = dateUtil::getAccrualFactor(_deposit->getSpotDate(),_deposit->getExpiryDate(), _deposit->getDayCount());
		double discountFactor = (1/(1+accrualFactor*_deposit->getRate()))*_spotDateDF;
		ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
		ai->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), std::get<0>(_curve->getCurveStartPoint()));
	}else{
		AbstractNumerical<DepositRateBootStrapper>* an = NumericalFactory<DepositRateBootStrapper>::getInstance()->getNumerical(this,&DepositRateBootStrapper::numericalFunc,_numericAlgo);
		double previousVal = std::get<1>(_startPoint);
		double lowerBound = abs(previousVal*(1-_plusMinus/100.0));
		double upperBound = previousVal*(1+_plusMinus/100.0);
		double discountFactor = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
		_spotDateDF = discountFactor;
		ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
		ai->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), std::get<0>(_startPoint));
	}
	return ai;
}

double DepositRateBootStrapper::numericalFunc(double x){
	AbstractInterpolator<date>* ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);
	ai->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), std::get<0>(_startPoint));

	date spotDate = _deposit->getSpotDate();
	double accrualFactor = dateUtil::getAccrualFactor(spotDate, _deposit->getExpiryDate(), _deposit->getDayCount());

	double spotDateDF = std::get<1>(ai->interpolate(spotDate));
	double paymentDateDF = std::get<1>(ai->interpolate(_deposit->getDeliveryDate()));

	double shouldBeZero = (1/(1+_deposit->getRate()*accrualFactor))*spotDateDF - paymentDateDF;
	return shouldBeZero;
}