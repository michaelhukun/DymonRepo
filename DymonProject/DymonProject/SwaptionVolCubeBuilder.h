//created by Wang Jianwei on 01 Dec 2012
//Added cashflowleg creating - Kun
#ifndef SWAPTIONVOLSCUBEBUILDER_H
#define SWAPTIONVOLCUBEBUILDER_H
#include "SwaptionVolSurface.h"
#include "SwaptionVolCube.h"
#include "AbstractBuilder.h"
#include "Market.h"
#include <vector>

using namespace instruments;

namespace utilities{
	class SwaptionVolCubeBuilder: public AbstractBuilder{
		
	public:
		
		SwaptionVolCubeBuilder():AbstractBuilder(){}

		void init(Configuration* cfg);

		SwaptionVolCube* build(Configuration* cfg);

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