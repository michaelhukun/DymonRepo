//created by Wang Jianwei on 1 Dec 2012

#include "MarketFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "RecordHelper.h"
#include "EnumHelper.h"
#include <iostream>

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;

MarketFileSource::MarketFileSource():
	AbstractFileSource(){}

MarketFileSource::MarketFileSource(std::string persistDir, std::string fileName):
	AbstractFileSource(persistDir, fileName){}

MarketFileSource::~MarketFileSource(){}

void MarketFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("Market.file",true,"");
	_persistDir = cfg->getProperty("Market.path",false,"");
	AbstractFileSource::init(cfg);
}

void MarketFileSource::retrieveRecord(){
	AbstractFileSource::retrieveRecord();

	string value;
	string market;
	RecordHelper::MarketMap MarketMap;
	while (_inFile.good()){
		_inFile>>value;
		vector<string> vec = fileUtil::split(value,':');
		market = vec[0];
		vector<string> properties = fileUtil::split(vec[1],',');

		enums::MarketEnum MarketName = EnumHelper::getCcyEnum(market);
		enums::DayCountEnum dayCountCash = EnumHelper::getDayCountEnum(properties[0]);
		enums::DayCountEnum dayCountSwap = EnumHelper::getDayCountEnum(properties[1]);
		enums::DayRollEnum dayRollCash = EnumHelper::getDayRollEnum(properties[2]);
		enums::DayRollEnum dayRollSwap = EnumHelper::getDayRollEnum(properties[3]);
		enums::DayRollEnum accrualAdjustCash = EnumHelper::getDayRollEnum(properties[4]);
		enums::DayRollEnum accrualAdjustSwap = EnumHelper::getDayRollEnum(properties[5]);
		int businessDaysAfterSpot = std::stoi(properties[6]);

		RecordHelper::MarketTuple ccyTuple(dayCountCash, dayCountSwap, dayRollCash, dayRollSwap, accrualAdjustCash, accrualAdjustSwap, businessDaysAfterSpot);

		cout << market<< " -> "<< "DayCountCashConvention "<< dayCountCash<<endl;
		cout << market<< " -> "<< "DayCountSwapConvention "<< dayCountSwap<<endl;
		cout << market<< " -> "<< "DayRollCashConvention "<< dayRollCash<<endl;
		cout << market<< " -> "<< "DayRollSwapConvention "<< dayRollSwap<<endl;
		cout << market<< " -> "<< "AccrualAdjustCashConvention "<< accrualAdjustCash<<endl;
		cout << market<< " -> "<< "AccrualAdjustSwapConvention "<< accrualAdjustSwap<<endl;
		cout << market<< " -> "<< "BusinessDaysAfterSpot "<< businessDaysAfterSpot<<endl;

		MarketMap.insert(pair<enums::MarketEnum, RecordHelper::MarketTuple>(MarketName,ccyTuple));
	}	
	RecordHelper::getInstance()->setMarketMap(MarketMap);
	_inFile.close();
}
