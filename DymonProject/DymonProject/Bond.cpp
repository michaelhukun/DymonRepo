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

using namespace instruments;
using namespace utilities;
using namespace std;
using namespace enums;

Bond::Bond(Market market, date tradeDate, date issueDate, date maturityDate, double notional, double couponRate, DiscountCurve* bc, int couponFreq, int buildDirection){
	BaseBond(market, tradeDate, issueDate, maturityDate, notional, couponRate, couponFreq, buildDirection);
	_bc=bc;
};

Bond::Bond(Market market, date tradeDate, date issueDate, date maturityDate, date firstCouponDate, int tenorNumOfMonths, double couponRate, int couponFreq, Configuration* cfg, double cleanPrice, enums::DayCountEnum dayCount){
	int timeLineBuildDirection = std::stoi(cfg->getProperty("BondDiscountCurve."+market.getNameString()+".buildCashFlowDirection",false,"1"));
	bool rollAccuralDates =  cfg->getProperty("BondDiscountCurve."+market.getNameString()+".rollAccuralDates",false,"0")=="0"?false:true;
	_tenorNumOfMonths = tenorNumOfMonths;
	_dayCount=dayCount;
	_cleanPrice = cleanPrice;
	_firstCouponDate = firstCouponDate;

	BaseBond(market, tradeDate, issueDate, maturityDate, 100, couponRate, couponFreq, timeLineBuildDirection);
	_dirtyPrice = couponFreq==NaN?NaN:deriveDirtyPrice();
}

Bond::Bond(Market market, date tradeDate, date issueDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* bc, int couponFreq){
	
	setTradeDate(tradeDate);
	setIssueDate(issueDate);
	setMaturityDate(dateUtil::getEndDate(tradeDate,tenorNumOfMonths,market.getDayRollBondConvention(),market.getMarketEnum(),dateUtil::MONTH));

	BuilderCashFlowLeg* couponLegBuilder = new BuilderCashFlowLeg(enums::BOND, issueDate, tenorNumOfMonths,couponRate,notional, couponFreq, market.getMarketEnum());

	_couponLeg=couponLegBuilder->getCashFlowLeg();
	_bc=bc;
	_market=market;
	_couponRate=couponRate;
	_couponFreq=couponFreq;
	_tenorNumOfMonths=tenorNumOfMonths;	
}

void Bond::BaseBond(Market market, date tradeDate, date issueDate, date maturityDate, double notional, double couponRate, int couponFreq, int buildDirection){

	setTradeDate(tradeDate);
	setIssueDate(issueDate);
	setMaturityDate(maturityDate);

	_market=market;
	_couponRate=couponRate;
	_couponFreq=couponFreq;
	_YTM = NaN;

	BuilderCashFlowLeg* couponLegBuilder = new BuilderCashFlowLeg(enums::BOND, issueDate, maturityDate, _tenorNumOfMonths, couponRate, notional, couponFreq, market.getMarketEnum(), buildDirection);
	_couponLeg=couponLegBuilder->getCashFlowLeg();
	if (_couponLeg->getCashFlowIndexForAccrualEnd(_firstCouponDate)==NaN){
		throw "First coupon date is not found in cash flow leg!";
	}
}

double Bond::deriveDirtyPrice(){
	int firstCouponCashFlowIndex = _couponLeg->getCashFlowIndexForAccrualEnd(_firstCouponDate);
	cashflow firstCashFlow = _couponLeg->getCashFlowVector()[firstCouponCashFlowIndex];
	date refStartDate = firstCashFlow.getAccuralStartDate();
	date refEndDate = firstCashFlow.getAccuralEndDate();
	double accrualFactor = dateUtil::getAccrualFactor(refStartDate, _tradeDate, refStartDate, refEndDate, _market.getDayCountBondConvention());
	return _cleanPrice + _couponRate/_couponFreq*accrualFactor;
}

double Bond::getMPV(){
	return BondPricer::getMPV(_couponLeg,_bc);
}