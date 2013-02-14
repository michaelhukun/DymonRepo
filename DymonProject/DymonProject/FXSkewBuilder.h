//created by Wang Jianwei on 01 Dec 2012
//Added cashflowleg creating - Kun
#ifndef FXSkewBuilder_H
#define FXSkewBuilder_H
#include "AbstractCurve.h"
#include "AbstractBuilder.h"
#include "Market.h"
#include <vector>

typedef tuple<date, double> point;
using namespace instruments;

namespace utilities{
	class FXSkewBuilder: public AbstractBuilder{
		
	public:
		
		FXSkewBuilder(enums::CurrencyEnum ccyEnum):AbstractBuilder(){_market = Market(ccyEnum);}

		void init(Configuration* cfg);

		AbstractCurve<double>* build(Configuration* cfg);

		void buildQuadratic(AbstractCurve<double>* dc);

		// Getters and Setters
		Market getMarket(){return _market;}
		enums::interpolAlgo getInterpolAlgo(){return _interpolAlgo;}
		CurrencyPair getCurrencyPair(){ return _currencyPair;}

		void setMarket(Market market){_market = market;}
		void setInterpolAlgo(enums::interpolAlgo interpolAlgo){_interpolAlgo=interpolAlgo;}
		void setCurrencyPair(CurrencyPair currencyPair){_currencyPair = currencyPair;}

	private:

		Market _market;
		enums::interpolAlgo _interpolAlgo;
		date _curveStartDate;
	};
}
#endif