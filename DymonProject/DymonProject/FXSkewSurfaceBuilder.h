//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWSURFACEBUILDER_H
#define FXSKEWSURFACEBUILDER_H
#include "FXSkewSurface.h"
#include "AbstractBuilder.h"
#include "RecordHelper.h"
#include "Market.h"
#include "CcyPair.h"
#include <vector>

using namespace instruments;
using namespace utilities;

namespace utilities{
	class FXSkewSurfaceBuilder: public AbstractBuilder{
		
	public:
		
		FXSkewSurfaceBuilder():AbstractBuilder(){};
		FXSkewSurfaceBuilder(std::string ccyPairStr):AbstractBuilder(){_ccyPair = CcyPair(ccyPairStr);}
		~FXSkewSurfaceBuilder(){}

		void init(Configuration* cfg);

		FXSkewSurface* build(Configuration* cfg);

		//Getters and Setters
		Market getMarket(){return _market;}
		CcyPair getCcyPair(){ return _ccyPair;}

		void setMarket(Market market){_market = market;}
		void setCcyPair(CcyPair ccyPair){_ccyPair = ccyPair;}

	private:

		CcyPair _ccyPair;
		Market _market;

	};
}
#endif