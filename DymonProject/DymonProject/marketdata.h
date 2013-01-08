//created by Hu Kun on 29 Nov 2012
#ifndef MarketData_H
#define MarketData_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "instrumentValue.h"
#include "SwaptionVolCube.h"
#include "AbstractInterpolator.h"
#include "DiscountCurve.h"
#include "Configuration.h"
#include <map>
#include <tuple>
#include "Enums.h"

using namespace utilities;
using namespace instruments;
using namespace std;
using namespace enums;

namespace Markets {

	// Singlton class for MarketData
	class MarketData {
	public:
		static MarketData* getInstance();

		void buildAll();

		void buildDiscountCurve();
		void buildSwaptionVolCube();

		DiscountCurve* getDiscountCurve(){return _discountCurve;}
		SwaptionVolCube* getSwaptionVolCube(){return _swaptionVolCube;};
		
	protected:
		//private copy constructor
		MarketData(){};
		~MarketData();
		
	private:
		static bool instanceFlag;
		static MarketData *single;
		DiscountCurve* _discountCurve;
		SwaptionVolCube* _swaptionVolCube;
	};
}
#endif