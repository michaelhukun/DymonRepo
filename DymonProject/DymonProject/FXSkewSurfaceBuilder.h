//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWSURFACEBUILDER_H
#define FXSKEWSURFACEBUILDER_H
#include "FXSkewSurface.h"
#include "AbstractBuilder.h"
#include "RecordHelper.h"
#include "Market.h"
#include "CurrencyPair.h"
#include <vector>

using namespace instruments;
using namespace utilities;

namespace utilities{
	class FXSkewSurfaceBuilder: public AbstractBuilder{
		
	public:
		
		FXSkewSurfaceBuilder(std::string currencyPairStr):AbstractBuilder(){}

		void init(Configuration* cfg);

		FXSkewSurface* build(Configuration* cfg);

		//Getters and Setters
		Market getMarket(){return _market;}
		CurrencyPair getCurrencyPair(){ return _currencyPair;}
		enums::interpolAlgo getInterpolAlgo(){return _interpolAlgo;}

		void setMarket(Market market){_market = market;}
		void setCurrencyPair(CurrencyPair currencyPair){_currencyPair = currencyPair;}
		void setInterpolAlgo(enums::interpolAlgo interpolAlgo){_interpolAlgo=interpolAlgo;}


	private:

		CurrencyPair _currencyPair;
		Market _market;
		enums::interpolAlgo _interpolAlgo;
	};
}
#endif