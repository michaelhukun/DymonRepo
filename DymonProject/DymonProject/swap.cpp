//created by Hu Kun 04 Dec 2012
//rewrote again with cashflow constructs by Kun 16 Dec 2012
#include "swap.h"
#include <iterator>
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "BuilderCashFlowLeg.h"
#include "RecordHelper.h"
#include "SwapPricer.h"
#include "AbstractPricer.h"

using namespace instruments;
using namespace utilities;
using namespace std;
using namespace enums;

Swap::Swap(date tradeDate, date maturityDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market fixLegCurr, Market floatingLegCurr, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection) {

	setTradeDate(tradeDate);
	setDeliveryDate(maturityDate);

	BuilderCashFlowLeg* fixLegs = new BuilderCashFlowLeg(enums::SWAP, tradeDate, maturityDate, tenorNumOfMonths, couponRate, notional, paymentFreqFixLeg, fixLegCurr.getCurrencyEnum(), buildDirection);
	BuilderCashFlowLeg* floatLegs = new BuilderCashFlowLeg(enums::SWAP, tradeDate, maturityDate, tenorNumOfMonths, yc ,notional, paymentFreqFloatingLeg, floatingLegCurr.getCurrencyEnum(), buildDirection);

	_fixCashflowLeg=fixLegs->getCashFlowLeg();
	_floatingCashflowLeg=floatLegs->getCashFlowLeg();
	_yc=yc;
	_fixLegCurr=fixLegCurr;
	_floatingLegCurr=floatingLegCurr;
	_paymentFreqFixLeg=paymentFreqFixLeg;
	_paymentFreqFloatingLeg=paymentFreqFloatingLeg;
}

Swap::Swap(date tradeDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market fixLegCurr, Market floatingLegCurr, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates) {
	
	setTradeDate(tradeDate);
	setDeliveryDate(dateUtil::getEndDate(tradeDate,tenorNumOfMonths,fixLegCurr.getDayRollSwapConvention(),fixLegCurr.getCurrencyEnum(),dateUtil::MONTH));

	BuilderCashFlowLeg* fixLegs = new BuilderCashFlowLeg(enums::SWAP,tradeDate, tenorNumOfMonths,couponRate,notional, paymentFreqFixLeg, fixLegCurr.getCurrencyEnum());
	BuilderCashFlowLeg* floatLegs= new BuilderCashFlowLeg(enums::SWAP,tradeDate, tenorNumOfMonths,yc,notional, paymentFreqFloatingLeg, floatingLegCurr.getCurrencyEnum());

	_fixCashflowLeg=fixLegs->getCashFlowLeg();
	_floatingCashflowLeg=floatLegs->getCashFlowLeg();
	_yc=yc;
	_fixLegCurr=fixLegCurr;
	_floatingLegCurr=floatingLegCurr;
	_paymentFreqFixLeg=paymentFreqFixLeg;
	_paymentFreqFloatingLeg=paymentFreqFloatingLeg;
	_tenorNumOfMonths=tenorNumOfMonths;	
}

Market Swap::getFixLegCurr() {
	return _fixLegCurr;
}

Market Swap::getFloatLegCurr() {
	return _floatingLegCurr;
}

int Swap::getPaymentFreqFixLeg() {
	return _paymentFreqFixLeg;
}

int Swap::getPaymentFreqFloatingLeg() {
	return _paymentFreqFloatingLeg;
}

cashflowLeg* Swap::getCashFlowVectorFix() {
	return _fixCashflowLeg;
}

cashflowLeg* Swap::getCashFlowVectorFloat() {
	return _floatingCashflowLeg;
}

void Swap::printCashflowLegFix() {
	_fixCashflowLeg->printCashFlowLeg();
}

void Swap::printCashflowLegFloat() {
	_floatingCashflowLeg->printCashFlowLeg();
}

DiscountCurve* Swap::getDiscountCurve() {
	return _yc;
}