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

Bond::Bond(Market market, date tradeDate, date maturityDate, double notional, double couponRate, DiscountCurve* bc, int couponFreq, bool rollAccuralDates, int buildDirection){
	BaseBond(market, tradeDate, maturityDate, notional, couponRate, couponFreq, rollAccuralDates, buildDirection);
	_bc=bc;
};

Bond::Bond(Market market, date tradeDate, date maturityDate, int tenorNumOfMonths, double couponRate, int couponFreq, Configuration* cfg, double cleanPrice, double YTM, enums::DayCountEnum dayCount){
	int timeLineBuildDirection = std::stoi(cfg->getProperty("BondDiscountCurve."+market.getNameString()+".buildCashFlowDirection",false,"1"));
	bool rollAccuralDates =  cfg->getProperty("BondDiscountCurve."+market.getNameString()+".rollAccuralDates",false,"0")=="0"?false:true;

	BaseBond(market, tradeDate, maturityDate, 100, couponRate, couponFreq, rollAccuralDates, timeLineBuildDirection);
	_cleanPrice = cleanPrice;
	_dirtyPrice = couponFreq==NaN?NaN:deriveDirtyPrice();
	_tenorNumOfMonths = tenorNumOfMonths;
	_YTM=YTM;
	_dayCount=dayCount;
}

Bond::Bond(Market market, date tradeDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* bc, int couponFreq, bool rollAccuralDates){
	
	setTradeDate(tradeDate);
	setMaturityDate(dateUtil::getEndDate(tradeDate,tenorNumOfMonths,market.getDayRollSwapConvention(),market.getMarketEnum(),dateUtil::MONTH));

	BuilderCashFlowLeg* couponLegs = new BuilderCashFlowLeg(enums::BOND,tradeDate, tenorNumOfMonths,couponRate,notional, couponFreq, market.getMarketEnum());

	_couponLeg=couponLegs->getCashFlowLeg();
	_bc=bc;
	_market=market;
	_couponRate=couponRate;
	_couponFreq=couponFreq;
	_tenorNumOfMonths=tenorNumOfMonths;	
}

void Bond::BaseBond(Market market, date tradeDate, date maturityDate, double notional, double couponRate, int couponFreq, bool rollAccuralDates, int buildDirection){

	setTradeDate(tradeDate);
	setMaturityDate(maturityDate);

	BuilderCashFlowLeg* couponLegs = new BuilderCashFlowLeg(enums::BOND,tradeDate, maturityDate,couponRate,notional, couponFreq, market.getMarketEnum(),buildDirection);

	_couponLeg=couponLegs->getCashFlowLeg();
	_market=market;
	_couponRate=couponRate;
	_couponFreq=couponFreq;
}

double Bond::deriveDirtyPrice(){
	cashflow firstCashFlow = _couponLeg->getCashFlowLeg()[0];
	date refStartDate = firstCashFlow.getAccuralStartDate();
	date refEndDate = firstCashFlow.getAccuralEndDate();
	double accrualFactor = dateUtil::getAccrualFactor(refStartDate,_tradeDate,refStartDate, refEndDate, _market.getDayCountBondConvention());
	return _cleanPrice + _couponRate/_couponFreq*accrualFactor;
}

double Bond::getMPV(){
	return BondPricer::getMPV(_couponLeg,_bc);
}