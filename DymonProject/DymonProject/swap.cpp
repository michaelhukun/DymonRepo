//created by Hu Kun 04 Dec 2012
//rewrote again with cashflow constructs by Kun 16 Dec 2012
#include "Swap.h"
#include <iterator>
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "CashFlowLegBuilder.h"
#include "AbstractPricer.h"

using namespace instruments;
using namespace utilities;
using namespace std;
using namespace enums;

void Swap::deriveDates(){
	_expiryDate = date(_spotDate.getJudianDayNumber()+_daysToMty);
	_deliveryDate = dateUtil::dayRollAdjust(_expiryDate, _market.getDayRollSwapConvention(), _market.getCurrencyEnum());
}

void Swap::buildFixedLeg(){
	setDayCountFixed(_market.getDayCountSwapFixed());
	setPayFreqFixed(_market.getPayFreqSwapFixed());
	_fixedCashflowLeg.setCashFlowNumber(_tenorInYear*_market.getPayFreqSwapFixed());
	CashFlowLegBuilder builder = CashFlowLegBuilder(this);
	builder.setPaymentFreq(getPayFreqFixed());
	builder.setDayCountEnum(getDayCountFixed());
	builder.setPaymentNumber(getCouponNumberFixed());
   builder.setBuildDirection(1);
	_fixedCashflowLeg.setCashFlowVector(*builder.build());
}

void Swap::buildFloatLeg(){
	setDayCountFloat(_market.getDayCountSwapFloat());
	setPayFreqFloat(_market.getPayFreqSwapFloat());
	_floatCashflowLeg.setCashFlowNumber(_tenorInYear*_market.getPayFreqSwapFloat());
	CashFlowLegBuilder builder = CashFlowLegBuilder(this);
	builder.setPaymentFreq(getPayFreqFloat());
	builder.setDayCountEnum(getDayCountFloat());
	builder.setPaymentNumber(getCouponNumberFloat());
   builder.setBuildDirection(1);
	_floatCashflowLeg.setCashFlowVector(*builder.build());
}

void Swap::printCashflowLegFix() {
	_fixedCashflowLeg.printCashFlowLeg();
}

void Swap::printCashflowLegFloat() {
	_floatCashflowLeg.printCashFlowLeg();
}