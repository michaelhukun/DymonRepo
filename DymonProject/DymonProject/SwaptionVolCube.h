//created by Wang Jianwei on 01 Dec 2012
#ifndef SWAPTIONVOLCUBE_H
#define SWAPTIONVOLCUBE_H
#include "AbstractSurface.h"
#include "SwaptionVolSurface.h"
#include "AbstractCurve.h"
#include "Market.h"
#include "Enums.h"

using namespace utilities;
using namespace instruments;

namespace utilities{
	class SwaptionVolCube: public AbstractDataStructure{
		
	public:		

		typedef map<double, SwaptionVolSurface*>::iterator CubeIterator;

		SwaptionVolCube():AbstractDataStructure(){
			_cube = new map<int,SwaptionVolSurface*>();
		};

		SwaptionVolCube(map<int,SwaptionVolSurface*>* cube, Market market, enums::interpolAlgo interpolAlgo){
			_cube=cube;
			_market=market;
			_interpolAlgo=interpolAlgo;
		};

		double getVol(double strike, double maturityInMonth, int tenorInMonth);

		SwaptionVolSurface* getVolSurface(int strike);

		AbstractCurve<double>* getVolCurveAlongStrike(double maturityInMonth, int tenorInMonth);

		void insertVolSurface(int strike, SwaptionVolSurface* volSurface){
			_cube->insert(std::make_pair(strike,volSurface));
		}

		map<int,SwaptionVolSurface*>* getCube(){return _cube;}
		void setCube(map<int,SwaptionVolSurface*>* cube){_cube = cube;}

		Market getMarket(){return _market;}
		void setMarket(Market market){_market = market;}

		enums::interpolAlgo getInterpolAlgo(){return _interpolAlgo;}
		void setInterpolAlgo(enums::interpolAlgo interpolAlgo){_interpolAlgo=interpolAlgo;}


	private:

		Market _market;
		enums::interpolAlgo _interpolAlgo;

		map<int,SwaptionVolSurface*>* _cube;

		AbstractInterpolator<double>* getVolLineSectionAlongStrike(int startStrike, double startVol, int endStrike, double endVol);
	};
}
#endif