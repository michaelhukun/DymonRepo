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
		AbstractOption(Market market, date tradeDate, int expiryInMonth, OptionType OptionTypeFlag, double S, double K, double vol){
			BaseOption(market, tradeDate, expiryInMonth, OptionTypeFlag, S, K, vol);
		}

		AbstractOption(Market market, date tradeDate, date expiryDate, OptionType OptionTypeFlag, double S, double K, double vol) {
			BaseOption(market, tradeDate, expiryDate, OptionTypeFlag, S, K, vol);
		}

		AbstractOption(Market market, date tradeDate, int expiryInMonth, OptionType OptionTypeFlag, double S, double K, double vol, double r) {
			BaseOption(market, tradeDate, expiryInMonth, OptionTypeFlag, S, K, vol);
			_discountFactor = exp(-r*expiryInMonth/12);
			_r=r;
		}

		virtual double getMPV(){return OptionPricer::getMPV();};
		double getVol(){return _vol;}

		void setDiscountFactor(double discountFactor){	_discountFactor = discountFactor;}

	protected:
		OptionType _OptionTypeFlag;
		double _S;
		double _K;
		double _vol;
		double _r;
		double _expiryInMonth;
		double _discountFactor;
		Market _market;

		void BaseOption(Market market, date tradeDate, int expiryInMonth, OptionType OptionTypeFlag, double S, double K, double vol){
			BaseOption(market, tradeDate, OptionTypeFlag, S, K, vol);
			setMaturityDate(dateUtil::getEndDate(tradeDate,expiryInMonth, enums::Mfollowing,market.getCurrencyEnum(),dateUtil::MONTH));			
			_expiryInMonth = expiryInMonth;
		}

		void BaseOption(Market market, date tradeDate, date expiryDate, OptionType OptionTypeFlag, double S, double K, double vol) {
			BaseOption(market, tradeDate, OptionTypeFlag, S, K, vol);
			setMaturityDate(expiryDate);
		}

		void BaseOption(Market market, date tradeDate, OptionType OptionTypeFlag, double S, double K, double vol) {
			setTradeDate(tradeDate);
			_OptionTypeFlag = OptionTypeFlag;
			_S = S;
			_K = K;
			_vol = vol;
			_market = market;
		}
	};
}
#endif