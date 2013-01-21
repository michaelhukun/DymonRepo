//created by Jianwei on 30 Nov 2012
#ifndef ABSTRACTOPTION_H
#define ABSTRACTOPTION_H
#include "AbstractInstrument.h"
#include "OptionPricer.h"
#include "Enums.h"
#include "Market.h"
#include "DiscountCurve.h"

using namespace utilities;
using namespace instruments;

namespace instruments {
	class AbstractOption:  public OptionPricer, public AbstractInstrument{

	public:

		AbstractOption(){};
		~AbstractOption(){};
		AbstractOption(Market market, date tradeDate, int expiryInMonth, CallPut callPutFlag, double S, double K, double vol){
			BaseOption(market, tradeDate, expiryInMonth, callPutFlag, S, K, vol);
		}

		AbstractOption(Market market, date tradeDate, date expiryDate, CallPut callPutFlag, double S, double K, double vol) {
			BaseOption(market, tradeDate, expiryDate, callPutFlag, S, K, vol);
		}

		AbstractOption(Market market, date tradeDate, int expiryInMonth, CallPut callPutFlag, double S, double K, double vol, double r) {
			BaseOption(market, tradeDate, expiryInMonth, callPutFlag, S, K, vol);
			_discountFactor = exp(-r*expiryInMonth/12);
			_r=r;
		}

		virtual double getMPV(){return OptionPricer::getMPV();};
		double getVol(){return _vol;}

		void setDiscountFactor(double discountFactor){	_discountFactor = discountFactor;}

	protected:
		CallPut _callPutFlag;
		double _S;
		double _K;
		double _vol;
		double _r;
		double _expiryInMonth;
		double _discountFactor;
		Market _market;

		void BaseOption(Market market, date tradeDate, int expiryInMonth, CallPut callPutFlag, double S, double K, double vol){
			BaseOption(market, tradeDate, callPutFlag, S, K, vol);
			setMaturityDate(dateUtil::getEndDate(tradeDate,expiryInMonth, enums::Mfollowing,market.getMarketEnum(),dateUtil::MONTH));			
			_expiryInMonth = expiryInMonth;
		}

		void BaseOption(Market market, date tradeDate, date expiryDate, CallPut callPutFlag, double S, double K, double vol) {
			BaseOption(market, tradeDate, callPutFlag, S, K, vol);
			setMaturityDate(expiryDate);
		}

		void BaseOption(Market market, date tradeDate, CallPut callPutFlag, double S, double K, double vol) {
			setTradeDate(tradeDate);
			_callPutFlag = callPutFlag;
			_S = S;
			_K = K;
			_vol = vol;
			_market = market;
		}
	};
}
#endif