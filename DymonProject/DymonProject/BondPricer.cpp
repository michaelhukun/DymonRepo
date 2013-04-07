#include "BondPricer.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"
#include "Configuration.h"
#include "marketdata.h"

using namespace instruments;
using namespace utilities;
using namespace Markets;

BondPricer::BondPricer(Bond* bond){
	init(Configuration::getInstance());
	_bond = bond;
}

void BondPricer::init(Configuration* cfg){
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration",false,"50"));
	_lowerBound = std::stod(cfg->getProperty("numerical.bond.lowerBound",false,"-0.01"));
	_upperBound = std::stod(cfg->getProperty("numerical.bond.upperBound",false,"0.07"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance",false,"0.0000001"));
}

double BondPricer::getMPV(DiscountCurve* discountCurve){
	return 0;
}

double BondPricer::getMPV(){
	double MPV = 0;
	if (_bond->getIsBill()){
		cashflow* cashFlowAtMaturity = _bond->getCouponLeg()->getCashFlow(0);
		date accrualStart = _bond->getSpotDate();
		date accrualEnd = cashFlowAtMaturity->getAccuralEndDate();
		date refStart = cashFlowAtMaturity->getAccuralStartDate();
		date refEnd = accrualEnd;
		//As this is a T-Bill it is quoted in terms of the discount rate.
		//The actual price of a T Bill is calculated as 100-(days to maturity/360)*Discount rate
		double accrualFactor = dateUtil::getAccrualFactor(accrualStart, accrualEnd, refStart, refEnd, _bond->getDayCount());
		double discountFactor = 1-accrualFactor*_bond->getCleanPrice()/100;
		return discountFactor*100;
	}else{
		vector<cashflow> couponLegVec = _bond->getCouponLeg()->getCashFlowVector();
		for (unsigned int i=_bond->getNextCouponIndex(); i<couponLegVec.size();i++){
			cashflow coupon = couponLegVec[i];
			date paymentDate = coupon.getPaymentDate();
			double dcf = _discountCurve->getDiscountFactor(_bond->getSpotDate(), paymentDate);
			double couponAmount = _bond->getCouponRate()*coupon.getNotional();
			double cashFlowAmt = couponAmount +((i==(couponLegVec.size()-1))?coupon.getNotional():0);
			MPV = MPV +	cashFlowAmt*dcf;
		}
	}
	return MPV;
}

double BondPricer::getYieldByDirtyPrice(double dirtyPrice){
	_tempTargetBondPrice = dirtyPrice;
	AbstractNumerical<BondPricer>* an = NumericalFactory<BondPricer>::getInstance()->getNumerical(this,&BondPricer::yieldSolverFunc,_numericAlgo);
	double yield = an->findRoot(_lowerBound,_upperBound,_tolerance,_iterateCount);
	return yield;
}

double BondPricer::getZeroRateSpread(double dirtyPrice){
	_tempTargetBondPrice = dirtyPrice;
	double lowerBond = -0.1;
	double upperBond = 0.1;
	AbstractNumerical<BondPricer>* an = NumericalFactory<BondPricer>::getInstance()->getNumerical(this,&BondPricer::curveBumpSolverFunc,_numericAlgo);
	double zeroRateSpread = an->findRoot(lowerBond,upperBond,_tolerance,_iterateCount);
	return zeroRateSpread;
}

double BondPricer::getYieldByZeroRate(double zerRate){
	double dayCountFraction = dateUtil::getAccrualFactor(_bond->getTradeDate(), _bond->getDeliveryDate(),_bond->getDayCount());
	double yield = (exp(dayCountFraction*zerRate)-1)/dayCountFraction;
	return yield;
}

double BondPricer::getYieldByDiscountFactor(double discountFactor){
	double dayCountFraction = dateUtil::getAccrualFactor(_bond->getTradeDate(), _bond->getDeliveryDate(),_bond->getDayCount());
	double yield = (1/discountFactor-1)/dayCountFraction;
	return yield;
}

double BondPricer::yieldSolverFunc(double yield){
	vector<cashflow> couponLegVec = _bond->getCouponLeg()->getCashFlowVector();
	double fullPrice = 0;
	int couponFreq = _bond->getCouponFreq();
	for (unsigned int i=_bond->getNextCouponIndex(); i< couponLegVec.size();i++){
		cashflow cf = couponLegVec[i];
		int paidCouponNum = i + 1;
		double yearToExpiry = paidCouponNum - _bond->getFractionFirstCouponAccrued();
		double dcf = pow(1 + (yield /couponFreq), -yearToExpiry );
		fullPrice = fullPrice + cf.getCouponRate()*cf.getNotional()*dcf;
		if (i == couponLegVec.size()-1) 
			fullPrice = fullPrice + cf.getNotional()*dcf;
	}
	double diff = fullPrice - _tempTargetBondPrice;
	return diff;
}

double BondPricer::curveBumpSolverFunc(double zeroRateSpread){
	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve(_bond->getMarket().getCurrencyEnum());
	vector<cashflow> couponLegVec = _bond->getCouponLeg()->getCashFlowVector();
	double fullPrice = 0;
	int couponFreq = _bond->getCouponFreq();
	for (unsigned int i=_bond->getNextCouponIndex(); i< couponLegVec.size();i++){
		cashflow coupon = couponLegVec[i];
		double discountFactor = dc->getDFChangingZeroRate(coupon.getAccuralEndDate(),zeroRateSpread,_bond->getDayCount());
		fullPrice = fullPrice + coupon.getCouponRate()*coupon.getNotional()*discountFactor;
		if (i == couponLegVec.size()-1) 
			fullPrice = fullPrice + coupon.getNotional()*discountFactor;
	}
	double diff = fullPrice - _tempTargetBondPrice;
	return diff;
}