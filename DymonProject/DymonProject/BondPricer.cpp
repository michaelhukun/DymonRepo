#include "BondPricer.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"
#include "Configuration.h"

using namespace instruments;
using namespace utilities;

BondPricer::BondPricer(Bond* bond){
	init(Configuration::getInstance());
	_bond = bond;
}

void BondPricer::init(Configuration* cfg){
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration",false,"50"));
	_lowerBound = std::stod(cfg->getProperty("numerical.bond.lowerBound",false,"-1"));
	_upperBound = std::stod(cfg->getProperty("numerical.bond.upperBound",false,"7"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance",false,"0.0000001"));
}

double BondPricer::getMPV(DiscountCurve* discountCurve){
	double MPV = 0;
	vector<cashflow> couponLegVec = _bond->getCouponLeg()->getCashFlowVector();
	for (unsigned int i=_bond->getNextCouponIndex(); i<couponLegVec.size();i++){
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
		int paidCouponNum = i + 1;
		double yearToExpiry = paidCouponNum - _bond->getFractionFirstCouponAccrued();
		double dcf = pow(1 + (yield / 100 /couponFreq), -yearToExpiry );
		fullPrice = fullPrice + cf.getCouponRate()*cf.getNotional()*dcf;
		if (i == couponLegVec.size()-1) 
			fullPrice = fullPrice + cf.getNotional()*dcf;
	}
	double diff = fullPrice - _tempTargetBondPrice;
	return diff;
}