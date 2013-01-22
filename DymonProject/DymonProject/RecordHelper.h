//created by Wang Jianwei on 01 Dec 2012
#ifndef RecordHelper_H
#define RecordHelper_H
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <tuple>
#include "AbstractSession.h"
#include "Enums.h"
#include "Market.h"
#include "bond.h"
#include "BondFuture.h"

using namespace utilities;
using namespace enums;
using namespace instruments;

namespace Session {
	class RecordHelper: public AbstractSession{
	
	public:
		
		static RecordHelper* getInstance();

		void init(Configuration*);

		typedef std::map<enums::MarketEnum, std::set<long>> HolidayMap;
		
		typedef std::map<enums::MarketEnum, std::map<long, double>> RateMap;
		
		typedef std::map<enums::MarketEnum, std::map<long, Bond>> BondRateMap;
		
		typedef std::map<enums::MarketEnum, std::map<long, BondFuture>> BondFutureMap;

		typedef std::map<enums::MarketEnum, Market> MarketMap;

		//std::map<BasisPoint,std::map<fSwapTenorNumOfMonths,map<optionTenorNumOfMonths,swaptionVol>>> SwaptionCubeMap
		typedef std::map<int,std::map<int, std::map<int,double>>> SwaptionCubeMap;

		typedef std::map<int, std::map<int,double>> SwaptionSurfaceMap;

		//std::map<tuple<fSwapTenorNumOfMonths,optionTenorNumOfMonths>,ATM Strike> SwaptionATMStrikeMap
		typedef std::map<std::tuple<int,int>,double> SwaptionATMStrikeMap;
		
		HolidayMap getHolidayMap(){return _holidayMap;}
		void setHolidayMap(HolidayMap map){_holidayMap=map;}

		RateMap getDepositRateMap(){return _depositRateMap;}
		void setDepositRateMap(RateMap map){_depositRateMap=map;}

		RateMap getOverNightRateMap(){return _overNightRateMap;}
		void setOverNightRateMap(RateMap map){_overNightRateMap=map;}

		BondRateMap* getBondRateMap(){return &_bondRateMap;}
		void setBondRateMap(BondRateMap map){_bondRateMap=map;}

		BondFutureMap* getBondFutureMap(){return &_bondFutureMap;}
		void setBondFutureMap(BondFutureMap map){_bondFutureMap=map;}

		RateMap getSwapRateMap(){return _swapRateMap;}
		void setSwapRateMap(RateMap map){_swapRateMap=map;}

		MarketMap getMarketMap(){return _MarketMap;}
		void setMarketMap(MarketMap map){_MarketMap=map;}

		SwaptionCubeMap getSwaptionVolMap(){return _swaptionCubeMap;}
		void setSwaptionVolMap(SwaptionCubeMap map){_swaptionCubeMap=map;}

		SwaptionATMStrikeMap getSwaptionATMStrikeMap(){return _swaptionATMStrikeMap;}
		void setSwaptionATMStrikeMap(SwaptionATMStrikeMap map){_swaptionATMStrikeMap=map;}

	private:		
		
		RecordHelper();
		~RecordHelper();

		static RecordHelper* single;

		HolidayMap _holidayMap;

		RateMap _depositRateMap;

		RateMap _overNightRateMap;

		RateMap _swapRateMap;

		BondRateMap _bondRateMap;

		BondFutureMap _bondFutureMap;
				
		MarketMap _MarketMap;

		SwaptionCubeMap _swaptionCubeMap;

		SwaptionATMStrikeMap _swaptionATMStrikeMap;
		
		void buildConfiguration(Configuration*);
		void buildHolidayMap(Configuration*);
		void buildSwapRateMap(Configuration*);
		void buildDepositRateMap(Configuration*);
		void buildMarketMap(Configuration*);
		void buildSwaptionVolMap(Configuration*);
		void buildBondRateMap(Configuration*);
		void buildBondFutureMap(Configuration*);
	};
}

#endif