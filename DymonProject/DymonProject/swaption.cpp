//created by Hu Kun on 30 Nov 2012
#include "swaption.h"
#include "Enums.h"
#include "AbstractOption.h"
#include "Swap.h"
#include "dateUtil.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

Swaption::Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double K, SwaptionVolCube* vc,date swapStartDate, int tenorNumOfMonths,double notional, double couponRate, DiscountCurve* yc, Market fixLegCurr, Market floatingLegCurr, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection){
	Swap* _underlyingSwap= new Swap(swapStartDate, tenorNumOfMonths, notional, couponRate, yc, fixLegCurr, floatingLegCurr, paymentFreqFixLeg, paymentFreqFloatingLeg, rollAccuralDates, buildDirection);
	Swaption(market, PayReceiveInd, expiryInMonth, K, vc, yc, _underlyingSwap);	
}

Swaption::Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double K, SwaptionVolCube* vc,DiscountCurve* dc, Swap* underlyingSwap){
	_underlyingSwap = underlyingSwap;
	_tenorInMonth = _underlyingSwap->getTenor();
	double forwardParRate=underlyingSwap->getParRate(underlyingSwap->getCashflowLegFloat(),underlyingSwap->getCashflowLegFix(),dc);
	date tradeDate = dateUtil::getToday();
	double vol=vc->getVol(K,expiryInMonth,_tenorInMonth);

	//PayReceiver Indictor with respect to the fixed leg
	AbstractOption(market, tradeDate, expiryInMonth, PayReceiveInd == Payer?Call:Put, forwardParRate, K, vol, dc);
}
		
double Swaption::getMPV(){
	return blackFormula(_callPutFlag, _S, _K, _vol, _discountFactor, _expiryInMonth/12);
}