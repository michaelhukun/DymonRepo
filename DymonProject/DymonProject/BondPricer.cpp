#include "BondPricer.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"


using namespace instruments;
using namespace utilities;

void BondPricer::init(Configuration* cfg){
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration",true,"50"));
	_lowerBound = std::stoi(cfg->getProperty("numerical.bond.lowerBound",true,"-0.01"));
	_upperBound = std::stoi(cfg->getProperty("numerical.bond.upperBound",true,"0.07"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance",true,"0.0000001"));
}

double BondPricer::getMPV(DiscountCurve* discountCurve){
	double MPV = 0;
	vector<cashflow> couponLegVec = _bond->getCouponLeg()->getCashFlowVector();
	for (unsigned int i=0; i<couponLegVec.size();i++){
		cashflow coupon = couponLegVec[i];
		date paymentDate = coupon.getPaymentDate();
		double dcf = discountCurve->getDiscountFactor(paymentDate);
		double couponAmount = coupon.getCouponRate()*coupon.getNotional();
		double cashFlowAmt = couponAmount +((i==(couponLegVec.size()-1))?coupon.getNotional():0);
		MPV = MPV +	cashFlowAmt*dcf;
	}
	return MPV;
}

double BondPricer::getYieldByDirtyPrice(double bondPrice){
	_tempTargetBondPrice = bondPrice;
	AbstractNumerical<BondPricer>* an = NumericalFactory<BondPricer>::getInstance()->getNumerical(this,&BondPricer::yieldSolverFunc,_numericAlgo);
	double yield = an->findRoot(_lowerBound,_upperBound,_tolerance,_iterateCount);
	return yield;
}


double BondPricer::getYieldByZeroRate(double zerRate){
	double dayCountFraction = dateUtil::getAccrualFactor(_bond->getTradeDate(), _bond->getMaturityDate(),_bond->getDayCount());
	double yield = (exp(dayCountFraction*zerRate)-1)/dayCountFraction;
	return yield;
}

double BondPricer::getYieldByDiscountFactor(double discountFactor){
	double dayCountFraction = dateUtil::getAccrualFactor(_bond->getTradeDate(), _bond->getMaturityDate(),_bond->getDayCount());
	double yield = (1/discountFactor-1)/dayCountFraction;
	return yield;
}

double BondPricer::yieldSolverFunc(double yield){
	vector<cashflow> couponLegVec = _bond->getCouponLeg()->getCashFlowVector();
	double fullPrice = 0;
	int couponFreq = _bond->getCouponFreq();
	for (unsigned int i=_bond->getNextCouponIndex(); i< couponLegVec.size();i++){
		cashflow cf = couponLegVec[i];
		int remainingCouponNum = couponLegVec.size() - i;
		double dcf = pow(1 + yield / couponFreq, - remainingCouponNum + _bond->getFractionFirstCouponAccrued());
		fullPrice = fullPrice + cf.getCouponRate()*cf.getNotional()*dcf;
		if (i == couponLegVec.size()-1) 
			fullPrice = fullPrice + cf.getNotional()*dcf;
	}
	double diff = fullPrice - _tempTargetBondPrice;
	return diff;
}