//created by Wang Jianwei on 1 Dec 2012

#include "BondRateBootStrapper.h"
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
#include "BondPricer.h"

using namespace utilities;

AbstractInterpolator<date>* BondRateBootStrapper::bootStrap(){
	AbstractInterpolator<date>* ai;
	double discountFactor;
	if (_bond->getIsBill()){
		BondPricer pricer(_bond);
		discountFactor = pricer.getMPV();
	}else {
		AbstractNumerical<BondRateBootStrapper>* an = NumericalFactory<BondRateBootStrapper>::getInstance()->getNumerical(this,&BondRateBootStrapper::numericalFunc,_numericAlgo);
		double previousVal = std::get<1>(_startPoint);
		double lowerBound = 0;//abs(previousVal*(1-_plusMinus/100.0));
		double upperBound = previousVal*(1+_plusMinus/100.0);
		discountFactor = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
	}
	date curveStartDate = std::get<0>(_bond->getIsBill()?_startPoint:_curve->getCurveStartPoint());
	ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	ai->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), curveStartDate);
	return ai;
}

double BondRateBootStrapper::numericalFunc(double x){	
	_interpolant = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);
	_interpolant->addCurveConfig(_curve->getCurveRateType(), _curve->getInterpolRateType(), _curve->getDayCount(), std::get<0>(_curve->getCurveStartPoint()));
	_curve->insertLineSection(_interpolant);
	BondPricer pricer(_bond);
	pricer.setDiscountCurve(_curve);
	double derivedBondPrice = pricer.getMPV();
	_curve->removeSection(_curve->getSize()-1);
	return derivedBondPrice - _bond->getDirtyPrice();
}