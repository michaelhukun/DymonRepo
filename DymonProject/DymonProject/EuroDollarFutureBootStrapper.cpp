//created by Wang Jianwei on 1 Dec 2012

#include "EuroDollarFutureBootStrapper.h"
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
#include "EuroDollarFuturePricer.h"

using namespace utilities;

typedef AbstractBootStrapper<date> super;

void EuroDollarFutureBootStrapper::init(Configuration* cfg){
	super::init(cfg);
}

AbstractInterpolator<date>* EuroDollarFutureBootStrapper::bootStrap(){
	AbstractNumerical<EuroDollarFutureBootStrapper>* an = NumericalFactory<EuroDollarFutureBootStrapper>::getInstance()->getNumerical(this,&EuroDollarFutureBootStrapper::numericalFunc,_numericAlgo);
	double previousVal = std::get<1>(_startPoint);
	double lowerBound = abs(previousVal*(1-_plusMinus/100.0));
	double upperBound = previousVal*(1+_plusMinus/100.0);
	double discountFactor = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
	AbstractInterpolator<date>* ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	ai->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), std::get<0>(_curve->getCurveStartPoint()));
	return ai;
}

double EuroDollarFutureBootStrapper::numericalFunc(double x){
	_interpolant = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);
	_interpolant->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), std::get<0>(_curve->getCurveStartPoint()));
	_curve->insertLineSection(_interpolant);
	EuroDollarFuturePricer pricer(_future);
	pricer.setDiscountCurve(_curve);
	double derivedForwardLibor = pricer.deriveForwardLibor();
	_curve->removeSection(_curve->getSize()-1);

	double shouldBeZero = derivedForwardLibor - _future->getForwardLiborRate();
	return shouldBeZero;
}