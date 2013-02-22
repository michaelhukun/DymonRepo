//created by Wang Jianwei on 01 Dec 2012
//Added cashflowleg creating - Kun
#ifndef SWAPTIONVOLSCUBEBUILDER_H
#define SWAPTIONVOLCUBEBUILDER_H
#include "SwaptionVolSurface.h"
#include "SwaptionVolCube.h"
#include "AbstractBuilder.h"
#include "RecordHelper.h"
#include "Market.h"
#include <vector>

using namespace instruments;
using namespace utilities;

namespace utilities{
	class SwaptionVolCubeBuilder: public AbstractBuilder{
		
	public:
		
		SwaptionVolCubeBuilder(enums::CurrencyEnum ccyEnum):AbstractBuilder(){_market = Market(ccyEnum);}

		void init(Configuration* cfg);

		SwaptionVolCube* build(Configuration* cfg);

		Market getMarket(){return _market;}
		void setMarket(Market market){_market = market;}


	private:

		Market _market;

		SwaptionVolSurface* buildVolSurface(RecordHelper::SwaptionSurfaceMap volSurfaceMap);
		AbstractCurve<double>* buildVolCurve(map<int,double> curveMap);
	};
}
#endif