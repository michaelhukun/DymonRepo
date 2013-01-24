//created by Wang Jianwei
#include "BondFuture.h"		

using namespace instruments;

double BondFuture::getMPV(double costOfCarryRate){
	double CTDDirtyPrice = _CTDBond->getCleanPrice();
	vector<cashflow> couponVector = _CTDBond->getCouponLeg()->getCashFlowVector();
	_spotPrice = CTDDirtyPrice - getPVCouponBeforeExpiry(couponVector, costOfCarryRate);
	double invoiceAmt = Future::getMPV(costOfCarryRate);
	double futurePriceBeforeConvertion = invoiceAmt - getAccuredInterestOnExpiry(couponVector);
	double futurePriceAfterConvertion = futurePriceBeforeConvertion/_CTDConvertionFactor;
	return futurePriceAfterConvertion;
}

double BondFuture::getImpliedBondPrice(){
	return 0;
}


double BondFuture::getPVCouponBeforeExpiry(vector<cashflow> couponVector, double costOfCarryRate){
	double PVCouponBeforeExpiry = 0;	
	for (int i=_CTDBond->getNextCouponIndex(); i<couponVector.size();i++){
		cashflow coupon = couponVector[i];
		if (coupon.getAccuralEndDate()>_maturityDate)
			continue;

		int daysToTradeDate = dateUtil::getDaysBetween(_tradeDate,coupon.getAccuralEndDate());
		double couponAmt = coupon.getNotional()*coupon.getCouponRate();
		double discountFactor = exp(-costOfCarryRate*daysToTradeDate/360);
		PVCouponBeforeExpiry = PVCouponBeforeExpiry + couponAmt*discountFactor;
	}
	return PVCouponBeforeExpiry;
}

double BondFuture::getAccuredInterestOnExpiry(vector<cashflow> couponVector){
	for (unsigned int i=0; i<couponVector.size(); i++){
		cashflow coupon= couponVector[i];
		if (_tradeDate>=coupon.getAccuralStartDate() && _tradeDate<coupon.getAccuralEndDate()){
			double dayCountFraction = dateUtil::getAccrualFactor(coupon.getAccuralStartDate(),_maturityDate,coupon.getAccuralStartDate(),coupon.getAccuralEndDate(),_CTDBond->getDayCount());
			double accruedInterest = coupon.getNotional()*coupon.getCouponRate()*dayCountFraction;
			return accruedInterest;
		}
	}
	throw "Bond future expiry not in the tenor of the CTD bond";
}