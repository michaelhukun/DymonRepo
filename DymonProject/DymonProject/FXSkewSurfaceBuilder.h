//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWSURFACEBUILDER_H
#define FXSKEWSURFACEBUILDER_H
#include "FXSkewSurface.h"
#include "AbstractBuilder.h"
#include "RecordHelper.h"
#include "Market.h"
#include <vector>

using namespace instruments;
using namespace utilities;

namespace utilities{
	class FXSkewSurfaceBuilder: public AbstractBuilder{
		
	public:
		
		FXSkewSurfaceBuilder(enums::CurrencyEnum ccyEnum):AbstractBuilder(){_market = Market(ccyEnum);}

		void init(Configuration* cfg);

		FXSkewSurface* build(Configuration* cfg);

		Market getMarket(){return _market;}
		void setMarket(Market market){_market = market;}

		enums::interpolAlgo getInterpolAlgo(){return _interpolAlgo;}
		void setInterpolAlgo(enums::interpolAlgo interpolAlgo){_interpolAlgo=interpolAlgo;}


	private:

		Market _market;
		enums::interpolAlgo _interpolAlgo;
	};
}
#endif