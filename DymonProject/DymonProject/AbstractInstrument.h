//created by Hu Kun on 29 Nov 2012
#ifndef ABSTRACTINSTRUMENT_H
#define ABSTRACTINSTRUMENT_H
#include "Market.h"
#include "date.h"

using namespace utilities;

namespace instruments {
	class AbstractInstrument {
	
	public:
		//base class for all other instruments to be derived from
		AbstractInstrument(){};
		virtual date getTradeDate() {return _tradeDate;};
		virtual date getMaturityDate() {return _maturityDate;};
		virtual void setTradeDate(date tradeDate) {_tradeDate=tradeDate;};
		virtual void setMaturityDate(date maturityDate) {_maturityDate=maturityDate;};

	protected: 
		Market _domMarket;
		Market _forMarket;
		date _tradeDate;
		date _maturityDate;

		
	};
}
#endif