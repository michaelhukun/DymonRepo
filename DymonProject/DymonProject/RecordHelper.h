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

using namespace utilities;
using namespace enums;

namespace Session {
	class RecordHelper: public AbstractSession{
	
	public:
		
		static RecordHelper* getInstance();

		void init(Configuration*);

		typedef std::map<enums::MarketEnum, std::set<long>> HolidayMap;
		
		typedef std::map<enums::MarketEnum, std::map<long, double>> RateMap;
		
		typedef std::tuple<enums::DayCountEnum, enums::DayCountEnum, enums::DayRollEnum, enums::DayRollEnum, enums::DayRollEnum, enums::DayRollEnum, int> MarketTuple;

		typedef std::map<enums::MarketEnum, MarketTuple> MarketMap;

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
				
		MarketMap _MarketMap;

		SwaptionCubeMap _swaptionCubeMap;

		SwaptionATMStrikeMap _swaptionATMStrikeMap;
		
		void buildConfiguration(Configuration*);
		void buildHolidayMap(Configuration*);
		void buildSwapRateMap(Configuration*);
		void buildDepositRateMap(Configuration*);
		void buildMarketMap(Configuration*);
		void buildSwaptionVolMap(Configuration*);

	};
}

#endif