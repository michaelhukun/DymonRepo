//created by Hu Kun 04 Dec 2012
//rewrote again with cashflow constructs by Kun 16 Dec 2012
#include "Swap.h"
#include <iterator>
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "BuilderCashFlowLeg.h"
#include "AbstractPricer.h"

using namespace instruments;
using namespace utilities;
using namespace std;
using namespace enums;

Swap::Swap(date tradeDate, date maturityDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market market, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection) {

	setTradeDate(tradeDate);
	setDeliveryDate(maturityDate);

	BuilderCashFlowLeg* fixLegs = new BuilderCashFlowLeg(enums::SWAP, tradeDate, maturityDate, tenorNumOfMonths, couponRate, notional, paymentFreqFixLeg, market.getCurrencyEnum(), buildDirection);
	BuilderCashFlowLeg* floatLegs = new BuilderCashFlowLeg(enums::SWAP, tradeDate, maturityDate, tenorNumOfMonths, yc ,notional, paymentFreqFloatingLeg, market.getCurrencyEnum(), buildDirection);

	_fixedCashflowLeg=fixLegs->getCashFlowLeg();
	_floatCashflowLeg=floatLegs->getCashFlowLeg();
	_yc=yc;
	setPayFreqFixed(paymentFreqFixLeg);
	setPayFreqFloat(paymentFreqFloatingLeg);
}

Swap::Swap(date tradeDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market market, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates) {

	setTradeDate(tradeDate);
	setDeliveryDate(dateUtil::getEndDate(tradeDate,tenorNumOfMonths,market.getDayRollSwapConvention(),market.getCurrencyEnum(),dateUtil::MONTH));

	BuilderCashFlowLeg* fixLegs = new BuilderCashFlowLeg(enums::SWAP,tradeDate, tenorNumOfMonths,couponRate,notional, paymentFreqFixLeg, market.getCurrencyEnum());
	BuilderCashFlowLeg* floatLegs= new BuilderCashFlowLeg(enums::SWAP,tradeDate, tenorNumOfMonths,yc,notional, paymentFreqFloatingLeg, market.getCurrencyEnum());

	_fixedCashflowLeg=fixLegs->getCashFlowLeg();
	_floatCashflowLeg=floatLegs->getCashFlowLeg();
	_yc=yc;
	setPayFreqFixed(paymentFreqFixLeg);
	setPayFreqFloat(paymentFreqFloatingLeg);
	_tenorNumOfMonths=tenorNumOfMonths;	
}


void Swap::deriveDates(){
	_expiryDate = date(_spotDate.getJudianDayNumber()+_daysToMty);
	_deliveryDate = dateUtil::dayRollAdjust(_expiryDate, _market.getDayRollSwapConvention(), _market.getCurrencyEnum());
}

void Swap::buildFixedLeg(){
	BuilderCashFlowLeg* fixLegs = new BuilderCashFlowLeg(enums::SWAP, _tradeDate, tenorNumOfMonths,couponRate,notional, paymentFreqFixLeg, market.getCurrencyEnum());
}

void Swap::buildFloatLeg(){
}

void Swap::printCashflowLegFix() {
	_fixedCashflowLeg->printCashFlowLeg();
}

void Swap::printCashflowLegFloat() {
	_floatCashflowLeg->printCashFlowLeg();
}