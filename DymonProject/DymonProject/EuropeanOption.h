//created by Jianwei on 30 Nov 2012
#ifndef EUROPEANOPTION_H
#define EUROPEANOPTION_H
#include "OptionPricer.h"
#include "AbstractInstrument.h"
#include "AbstractOption.h"

using namespace instruments;

namespace instruments {
	class EuropeanOption:  public AbstractOption{
	public:
		// Constructors
		EuropeanOption():AbstractOption(){};
		~EuropeanOption(){};

		EuropeanOption(date tradeDate, date expiryDate, date deliveryDate, VolType volType, double S, double K, double vol):
		AbstractOption(tradeDate, expiryDate, deliveryDate, volType, S, K, vol){}

		EuropeanOption(Market market, date tradeDate, int expiryInMonth, VolType VolTypeFlag, double S, double K, double vol, double r):
		AbstractOption(market, tradeDate, expiryInMonth, VolTypeFlag, S, K, vol){
			setDiscountRate(r);
		}

		// Methods
		virtual double getMPV();
		virtual std::string toString(){return "";}
	};
}
#endif