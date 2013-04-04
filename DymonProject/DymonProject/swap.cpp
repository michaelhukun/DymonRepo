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
#include "Libor.h"

using namespace instruments;
using namespace utilities;
using namespace std;
using namespace enums;

void Swap::deriveDates(){
	_expiryDate = dateUtil::getEndDate(_spotDate,_tenorInYear,_market.getAccrualAdjustSwapConvention(), _market.getCurrencyEnum(), dateUtil::YEAR);
	_deliveryDate = dateUtil::dayRollAdjust(_expiryDate, _market.getDayRollSwapConvention(), _market.getCurrencyEnum());
}

void Swap::deriveDayCount(){
	_fixedCashflowLeg.setDayCount(_market.getDayCountSwapFixed());
	_floatCashflowLeg.setDayCount(_market.getDayCountSwapFloat());
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
	builder.setJoinMismatchedEndPoint(false);
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
	insertFloatLegReset();
}

void Swap::insertFloatLegReset(){
	for (int i=0; i<_floatCashflowLeg.getSize(); i++){
		cashflow* cf = _floatCashflowLeg.getCashFlow(i);
		Libor* libor = new Libor();
		Market market = Market(_market.getLiborCalendar());
		date liborFixingDate = dateUtil::getBizDateOffSet(cf->getAccuralStartDate(),-market.getBusinessDaysAfterSpotCash(),market.getCurrencyEnum());
		libor->setTenorNum(cf->getTenorInMonth());
		libor->setDateUnit(dateUtil::MONTH);
		libor->setMarket(market);
		libor->setDayCount(_market.getDayCountCashConvention());
		libor->setIsOverNight(false);
		libor->setFixingDate(liborFixingDate);
		libor->deriveDates();
		cf->setReset(*libor);
	}
}

date Swap::getMaxSwapAndResetDeliveryDate(){
	date lastResetDeliveryDate = (_floatCashflowLeg.getCashFlowVector().back()).getReset()->getDeliveryDate();
	if (_deliveryDate<lastResetDeliveryDate)
		return lastResetDeliveryDate;
	return _deliveryDate;
}

void Swap::printCashflowLegFix() {
	_fixedCashflowLeg.printCashFlowLeg();
}

void Swap::printCashflowLegFloat() {
	_floatCashflowLeg.printCashFlowLeg();
}