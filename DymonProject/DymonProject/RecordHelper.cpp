//created by Wang Jianwei on 01 Dec 2012

#include "RecordHelper.h"
#include "AbstractDao.h"
#include "Configuration.h"
#include "swapRateFileSource.h"
#include "HolidayFileSource.h"
#include "DepositFileSource.h"
#include "ConfigurationFileSource.h"
#include "MarketFileSource.h"
#include "SwaptionVolFileSource.h"
#include "BondPriceFileSource.h"
#include "BondFutureFileSource.h"

using namespace Session;
using namespace DAO;
using namespace std;

RecordHelper* RecordHelper::single = NULL;

RecordHelper* RecordHelper::getInstance()
{
	if(! single)
		single = new RecordHelper();
	return single;
}

RecordHelper::RecordHelper(){
}

void RecordHelper::init(Configuration* cfg){
	buildConfiguration(cfg);
	buildMarketMap(cfg);
	buildHolidayMap(cfg);
	buildSwapRateMap(cfg);
	buildDepositRateMap(cfg);	
	buildSwaptionVolMap(cfg);
	buildBondRateMap(cfg);
	buildBondFutureMap(cfg);
}

void RecordHelper::buildSwaptionVolMap(Configuration* cfg){
	AbstractDAO* SwaptionVolDataSource = new SwaptionVolFileSource();
	SwaptionVolDataSource->init(cfg);
	SwaptionVolDataSource->retrieveRecord();
}

void RecordHelper::buildSwapRateMap(Configuration* cfg){
	AbstractDAO* SwapRateDataSource = new SwapRateFileSource();
	SwapRateDataSource->init(cfg);
	SwapRateDataSource->retrieveRecord();
}

void RecordHelper::buildDepositRateMap(Configuration* cfg){
	AbstractDAO* depositRateDataSource = new DepositFileSource();
	depositRateDataSource->init(cfg);
	depositRateDataSource->retrieveRecord();
}

void RecordHelper::buildHolidayMap(Configuration* cfg){
	AbstractDAO* holidayDataSource = new HolidayFileSource();
	holidayDataSource->init(cfg);
	holidayDataSource->retrieveRecord();
}

void RecordHelper::buildMarketMap(Configuration* cfg){
	AbstractDAO* MarketDataSource= new MarketFileSource();
	MarketDataSource->init(cfg);
	MarketDataSource->retrieveRecord();
}

void RecordHelper::buildConfiguration(Configuration* cfg){
	AbstractDAO* configDataSource= new ConfigurationFileSource();
	configDataSource->init(cfg);
	configDataSource->retrieveRecord();
}

void RecordHelper::buildBondRateMap(Configuration* cfg){
	AbstractDAO* bondDataSource= new BondPriceFileSource();
	bondDataSource->init(cfg);
	bondDataSource->retrieveRecord();
}
		
void RecordHelper::buildBondFutureMap(Configuration* cfg){
	AbstractDAO* bondFutureDataSource= new BondFutureFileSource();
	bondFutureDataSource->init(cfg);
	bondFutureDataSource->retrieveRecord();
}