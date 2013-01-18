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

using namespace utilities;

AbstractInterpolator<date>* BondRateBootStrapper::bootStrap(){
	AbstractInterpolator<date>* ai;
	double discountFactor;
	if (_bond.getCouponFreq()==NaN){
		discountFactor = getTreasuryBillDiscountFactor();
	}else {
		AbstractNumerical<BondRateBootStrapper>* an = NumericalFactory<BondRateBootStrapper>::getInstance()->getNumerical(this,&BondRateBootStrapper::numericalFunc,_numericAlgo);
		double previousVal = std::get<1>(_startPoint);
		double lowerBound = 0;//abs(previousVal*(1-_plusMinus/100.0));
		double upperBound = previousVal*(1+_plusMinus/100.0);
		discountFactor = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
	}
	ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	return ai;
}

double BondRateBootStrapper::numericalFunc(double x){	
	AbstractInterpolator<date>* ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,x) , _interpolAlgo);

	vector<cashflow> couponLeg = _bond.getCouponLeg()->getCashFlowLeg();
	double derivedBondPrice = 0;

	for( unsigned int i=0; i<couponLeg.size(); i++){
		cashflow ithCashFlow = couponLeg[i];
		date paymentDate = ithCashFlow.getPaymentDate();
		date lastDateOnExistingCurve = std::get<0>(_startPoint);
		double ithDF = 0;
		if (paymentDate<=lastDateOnExistingCurve){
			ithDF = _curve->getDiscountFactor(paymentDate);
		}else {
			ithDF = std::get<1>(ai->interpolate(paymentDate));
		}
		double ithCouponAmt = ithDF*_couponRate*ithCashFlow.getNotional();
		double ithCashFlowAmt = ithCouponAmt + ithDF*(i==(couponLeg.size()-1)?ithCashFlow.getNotional():0);
		derivedBondPrice = derivedBondPrice + ithCashFlowAmt;
	}
	double priceDiff = derivedBondPrice - _bond.getDirtyPrice();

	return priceDiff;
}

double BondRateBootStrapper::getTreasuryBillDiscountFactor(){
	cashflow cashFlowAtMaturity = _bond.getCouponLeg()->getCashFlow(0);
	date accrualStart = _bond.getTradeDate();
	date accrualEnd = cashFlowAtMaturity.getAccuralEndDate();
	date refStart = cashFlowAtMaturity.getAccuralStartDate();
	date refEnd = accrualEnd;
	double accrualFactor = dateUtil::getAccrualFactor(accrualStart, accrualEnd, refStart, refEnd, _dayCount);
	return exp(-accrualFactor*_bond.getCleanPrice()/100);
}