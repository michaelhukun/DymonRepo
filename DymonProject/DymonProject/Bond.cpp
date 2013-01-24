//created by Wang Jianwei 04 Dec 2012
//rewrote again with cashflow constructs by Kun 16 Dec 2012
#include "bond.h"
#include <iterator>
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "BuilderCashFlowLeg.h"
#include "RecordHelper.h"
#include "AbstractPricer.h"
#include "Configuration.h"
#include "BondPricer.h"
#include <sstream>

using namespace instruments;
using namespace utilities;
using namespace std;
using namespace enums;

void Bond::generateCouponLeg(){
	Configuration* cfg = Configuration::getInstance();
	int buildDirection = std::stoi(cfg->getProperty("BondDiscountCurve."+_market.getNameString()+".buildCashFlowDirection",false,"-1"));
	BuilderCashFlowLeg* couponLegBuilder = new BuilderCashFlowLeg(enums::BOND, _issueDate, _maturityDate, _tenorNumOfMonths, _couponRate, 100, _couponFreq, _market.getMarketEnum(), buildDirection);
	_couponLeg=couponLegBuilder->getCashFlowLeg();
	_nextCouponDate = findNextCouponDate();
	_nextCouponIndex = _couponLeg->getCashFlowIndexForAccrualEnd(_nextCouponDate);
}

date Bond::findNextCouponDate(){
	if (!_nextCouponDate.isNull()) 
		return _nextCouponDate;

	vector<cashflow> couponVector = _couponLeg->getCashFlowVector();
	for (unsigned int i = 0; i< couponVector.size(); i++){
		cashflow coupon = couponVector.at(i);
		if (_tradeDate>=coupon.getAccuralStartDate() && _tradeDate<coupon.getAccuralEndDate())
			return coupon.getAccuralEndDate();
	}
	throw "Next coupon date not found!";
}

void Bond::deriveDirtyPrice(){
	if (_couponFreq==NaN ){
		_dirtyPrice = NaN;
	}else{
		if (_nextCouponIndex==NaN) throw "Next coupon index not found!";
		cashflow firstCashFlow = _couponLeg->getCashFlowVector()[_nextCouponIndex];
		date refStartDate = firstCashFlow.getAccuralStartDate();
		date refEndDate = firstCashFlow.getAccuralEndDate();
		_fractionFirstCouponAccrued = dateUtil::getAccrualFactor(refStartDate, _tradeDate, refStartDate, refEndDate, _market.getDayCountBondConvention());
		_dirtyPrice = _cleanPrice + _couponRate/_couponFreq*_fractionFirstCouponAccrued;
	}
}

double Bond::getMPV(DiscountCurve* bc){
	BondPricer pricer(this);
	double MPV = pricer.getMPV(bc);
	return MPV;
}

double Bond::getYield(){
	BondPricer pricer(this);
	double yield = NaN;
	if (_securityType=="Bill"){
		yield = pricer.getYieldByZeroRate(_cleanPrice);
	}else{
		yield = pricer.getYieldByDirtyPrice(_dirtyPrice);
	}
	return yield;
}

double Bond::getGspread(DiscountCurve* bc){
	BondPricer pricer(this);
	double gSpread = NaN;
	double yieldByBondCurve;
	double yieldByQuotedPrice = getYield();
	if (_securityType=="Bill"){
		date paymentDate = _couponLeg->getCashFlow(_couponLeg->getSize()-1).getPaymentDate();
		double discountFactor = bc->getDiscountFactor(paymentDate);
		yieldByBondCurve = pricer.getYieldByDiscountFactor(discountFactor);
	}else{
		double MPV = pricer.getMPV(bc);
		yieldByBondCurve = pricer.getYieldByDirtyPrice(MPV);
	}
		gSpread =  yieldByQuotedPrice - yieldByBondCurve;
	return gSpread;
}

string Bond::toString(){	
	std::stringstream ss (stringstream::in | stringstream::out);
	ss<<"BondID ["+_CUSIP+"], BondName ["+_name+"], Maturity ["+_maturityDate.toString()+"], quotedPrice ["<<_cleanPrice<<"], quotedYTM ["<<_quotedYTM<<"]";
	return ss.str();
}