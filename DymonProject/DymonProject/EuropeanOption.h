//created by Jianwei on 30 Nov 2012
#ifndef EUROPEANOPTION_H
#define EUROPEANOPTION_H
#include "OptionPricer.h"
#include "AbstractInstrument.h"
#include "AbstractOption.h"

using namespace instruments;
typedef AbstractOption super;

namespace instruments {
	class EuropeanOption:  public AbstractOption{
	public:
		EuropeanOption():AbstractOption(){};
		~EuropeanOption(){};
		EuropeanOption(Market market, date tradeDate, int expiryInMonth, CallPut callPutFlag, double S, double K, double vol, double r):
		AbstractOption(market, tradeDate, expiryInMonth, callPutFlag, S, K, vol, r){}

		virtual double getMPV();
	};
}
#endif