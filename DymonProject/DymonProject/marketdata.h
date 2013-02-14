//created by Hu Kun on 29 Nov 2012
#ifndef MarketData_H
#define MarketData_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "SwaptionVolCube.h"
#include "AbstractInterpolator.h"
#include "DiscountCurve.h"
#include "Configuration.h"
#include "FXSkewSurface.h"
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

		void buildBondDiscountCurve();
		void buildSwapDiscountCurve();
		void buildSwaptionVolCube();
		void buildFXSkewSurface();

		DiscountCurve* getSwapDiscountCurve(enums::CurrencyEnum market);
		DiscountCurve* getBondDiscountCurve(enums::CurrencyEnum market);
		SwaptionVolCube* getSwaptionVolCube(enums::CurrencyEnum market);
		FXSkewSurface* getFXSkewSurface(std::string currencyPairStr);
		
	protected:
		//private copy constructor
		MarketData(){};
		~MarketData();
		
	private:
		static bool instanceFlag;
		static MarketData *single;
		std::map<enums::CurrencyEnum, DiscountCurve> _SwapDiscountCurveMap;
		std::map<enums::CurrencyEnum, DiscountCurve> _BondDiscountCurveMap;
		std::map<enums::CurrencyEnum, SwaptionVolCube> _SwaptionVolCubeMap;
		std::map<std::string, FXSkewSurface> _FXSkewSurfaceMap;
	};
}
#endif