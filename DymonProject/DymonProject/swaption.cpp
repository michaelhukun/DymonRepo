//created by Hu Kun on 30 Nov 2012
#include "swaption.h"
#include "Enums.h"
#include "AbstractOption.h"
#include "Swap.h"
#include "dateUtil.h"
#include "marketdata.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;
using namespace Markets;

Swaption::Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, SwaptionVolCube* vc,DiscountCurve* dc, Swap* underlyingSwap){
	BaseSwaption(market, PayReceiveInd, expiryInMonth, strikeInBps, vc, dc, underlyingSwap);
}

Swaption::Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, Swap* underlyingSwap){
	SwaptionVolCube* vc = MarketData::getInstance()->getSwaptionVolCube(market.getCurrencyEnum());
	DiscountCurve* dc = MarketData::getInstance()->getSwapDiscountCurve(market.getCurrencyEnum());
	BaseSwaption(market,PayReceiveInd, expiryInMonth, strikeInBps, vc, dc, underlyingSwap);
}

Swaption::Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, int tenorInMonth){
	date swapStartDate = dateUtil::getEndDate(dateUtil::getToday(),expiryInMonth,enums::Mfollowing, market.getCurrencyEnum(), dateUtil::MONTH);
	double notional=1000000;
	double couponRate=0.03;
	int paymentFreqFixLeg=2;
	int paymentFreqFloatingLeg=4;
    bool rollAccuralDates=true;
	DiscountCurve* dc = MarketData::getInstance()->getSwapDiscountCurve(market.getCurrencyEnum());
	SwaptionVolCube* vc = MarketData::getInstance()->getSwaptionVolCube(market.getCurrencyEnum());
	Swap* underlyingSwap= new Swap(swapStartDate, tenorInMonth, notional, couponRate, dc, market, market, paymentFreqFixLeg, paymentFreqFloatingLeg, rollAccuralDates);
	
	BaseSwaption(market, PayReceiveInd, expiryInMonth, strikeInBps, vc, dc, underlyingSwap);
}

void Swaption::BaseSwaption(Market market, PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, SwaptionVolCube* vc, DiscountCurve* dc, Swap* underlyingSwap){
	_underlyingSwap = underlyingSwap;
	_tenorInMonth = _underlyingSwap->getTenor();
	cashflowLeg* floatCashflowLeg = underlyingSwap->getCashFlowVectorFloat();
	cashflowLeg* fixCashflowLeg = underlyingSwap->getCashFlowVectorFix();
	double forwardParRate=underlyingSwap->getParRate(floatCashflowLeg,fixCashflowLeg,dc);
	date tradeDate = dateUtil::getToday();
	double vol=vc->getVol(strikeInBps,expiryInMonth,_tenorInMonth);
	double strikeInDecimal = forwardParRate+strikeInBps/10000;	
	double discountFactor = getAnnuityMonetizer(dc);

	//PayReceiver Indictor with respect to the fixed leg
	BaseOption(tradeDate, PayReceiveInd == Payer?Call:Put, forwardParRate, strikeInDecimal, vol);
	setMarket(market);
	setExpiryInMonth(expiryInMonth);
	setDiscountCurve(dc);
}

double Swaption::getAnnuityMonetizer( DiscountCurve* dc) {

	cashflowLeg* swapFixCashflowLeg= _underlyingSwap->getCashFlowVectorFix();
	vector<date> accrualDates=swapFixCashflowLeg->getAccuralEndDates();
	DayCountEnum swapDayCount = _underlyingSwap->getFixLegCurr().getDayCountSwapConvention();
	double sum=0.0;
	for (vector<date>::iterator it=accrualDates.begin();it!=accrualDates.end();it++) {
		sum+=dateUtil::getAccrualFactor(dateUtil::getToday(),*it,swapDayCount)*(dc->getDiscountFactor(*it));
	}

	return sum;
}

double Swaption::getMPV(){
	DiscountCurve* dc = MarketData::getInstance()->getSwapDiscountCurve(_market.getCurrencyEnum());
	return blackFormula(_volType, _S, _K, _vol, _discountFactor, _expiryInMonth/12);
}

