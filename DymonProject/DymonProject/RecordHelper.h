//created by Wang Jianwei on 01 Dec 2012
#ifndef RecordHelper_H
#define RecordHelper_H
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <tuple>
#include "AbstractDao.h"
#include "AbstractSession.h"
#include "Enums.h"
#include "Market.h"
#include "bond.h"
#include "BondFuture.h"
#include "FXForward.h"
#include "FXEuropeanOption.h"
#include "Swap.h"
#include "Deposit.h"
#include "Libor.h"

using namespace utilities;
using namespace DAO;
using namespace enums;
using namespace instruments;

namespace Session {
	class RecordHelper: public AbstractSession{
	
	public:
		
		static RecordHelper* getInstance();

		void init(Configuration*);

		// Type Definition
		typedef std::map<enums::CurrencyEnum, std::set<long>> HolidayMap;	
		typedef std::map<enums::CurrencyEnum, std::map<long, Deposit>> DepositRateMap;	
		typedef std::map<enums::CurrencyEnum, std::map<long, Swap>> SwapRateMap;		
		typedef std::map<enums::CurrencyEnum, std::map<long, Bond>> BondRateMap;		
		typedef std::map<enums::CurrencyEnum, std::map<long, BondFuture>> BondFutureMap;
		typedef std::map<enums::CurrencyEnum, Market> MarketMap;
		//std::map<BasisPoint,std::map<fSwapTenorNumOfMonths,map<optionTenorNumOfMonths,swaptionVol>>> SwaptionCubeMap
		typedef std::map<int, std::map<int, std::map<int, double>>> SwaptionCubeMap;
		typedef std::map<int, std::map<int, double>> SwaptionSurfaceMap;
		//std::map<tuple<fSwapTenorNumOfMonths,optionTenorNumOfMonths>,ATM Strike> SwaptionATMStrikeMap
		typedef std::map<std::tuple<int, int>, double> SwaptionATMStrikeMap;
		typedef map<string, map<int, vector<FXEuropeanOption>>> FXVolSkewMap;
		typedef map<string, map<long, FXForward>> FXForwardMap;
		typedef std::map<string, CcyPair> CcyPairMap;
		
		// Getters
		HolidayMap getHolidayMap(){return _holidayMap;}
		DepositRateMap* getDepositRateMap(){return &_depositRateMap;}
		DepositRateMap* getLiborRateMap(){return &_liborRateMap;}
		BondRateMap* getBondRateMap(){return &_bondRateMap;}
		BondFutureMap* getBondFutureMap(){return &_bondFutureMap;}
		SwapRateMap* getSwapRateMap(){return &_swapRateMap;}
		MarketMap getMarketMap(){return _MarketMap;}
		SwaptionCubeMap getSwaptionVolMap(){return _swaptionCubeMap;}
		SwaptionATMStrikeMap getSwaptionATMStrikeMap(){return _swaptionATMStrikeMap;}
		FXVolSkewMap* getFXVolSkewMap(){ return &_FXVolSkewMap; }
		FXForwardMap* getFXForwardMap(){ return &_FXForwardMap; }
		CcyPairMap* getCcyPairMap(){ return &_CcyPairMap; }

		// Setters
		void setHolidayMap(HolidayMap map){_holidayMap=map;}
		void setDepositRateMap(DepositRateMap map){_depositRateMap=map;}
		void setLiborRateMap(DepositRateMap map){_liborRateMap=map;}
		void setBondRateMap(BondRateMap map){_bondRateMap=map;}
		void setBondFutureMap(BondFutureMap map){_bondFutureMap=map;}
		void setSwapRateMap(SwapRateMap map){_swapRateMap=map;}
		void setMarketMap(MarketMap map){_MarketMap=map;}
		void setSwaptionVolMap(SwaptionCubeMap map){_swaptionCubeMap=map;}
		void setSwaptionATMStrikeMap(SwaptionATMStrikeMap map){_swaptionATMStrikeMap=map;}
		void setFXVolSkewMap(FXVolSkewMap map){_FXVolSkewMap=map;}
		void setFXForwardMap(FXForwardMap map){_FXForwardMap=map;}
		void setCcyPairMap(CcyPairMap map){_CcyPairMap=map;}

		// Methods
		Bond* findCTDinBondMap(std::string CUSIP);
		date getSpotDate(string ccyPairStr);

	private:		
		
		RecordHelper(){};
		~RecordHelper(){};

		static RecordHelper* single;
		vector<AbstractDAO*> dataSourceVector;

		HolidayMap _holidayMap;
		DepositRateMap _depositRateMap;
		DepositRateMap _liborRateMap;
		SwapRateMap _swapRateMap;
		BondRateMap _bondRateMap;
		BondFutureMap _bondFutureMap;				
		MarketMap _MarketMap;
		SwaptionCubeMap _swaptionCubeMap;
		SwaptionATMStrikeMap _swaptionATMStrikeMap;
		FXVolSkewMap _FXVolSkewMap;
		FXForwardMap _FXForwardMap;
		CcyPairMap _CcyPairMap;
	};
}

#endif