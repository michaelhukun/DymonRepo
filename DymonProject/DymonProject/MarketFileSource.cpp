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

void MarketFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("marketConvention.file",true,"");
	_persistDir = cfg->getProperty("marketConvention.path",false,"");
	AbstractFileSource::init(cfg);
}

void MarketFileSource::retrieveRecord(){
	AbstractFileSource::retrieveRecord();

	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();
	RecordHelper::MarketMap marketMap;

	for (int j=1;j<numOfCols;j++) {

		String marketStr = db.at(0).at(j);
		enums::MarketEnum marketEnum = EnumHelper::getCcyEnum(marketStr);
		Market* market = new Market();

		for (int i=0;i<numOfRows;i++){
			String fieldName=db.at(i).at(0);
			String fieldVal=db.at(i).at(j);
			updateMarketObjectField(fieldName, fieldVal, market);
		}
		market->display();
		marketMap.insert(std::make_pair(marketEnum,*market));
	}
	RecordHelper::getInstance()->setMarketMap(marketMap);
	_inFile.close();
}

void MarketFileSource::updateMarketObjectField(std::string fieldName, std::string fieldVal, Market* market){

	if (fieldName=="Market"){
		market->setMarketEnum(EnumHelper::getCcyEnum(fieldVal));
	}else if (fieldName=="DayCountCash"){
		market->setDayCountCashConvention(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayCountSwap"){
		market->setDayCountSwapConvention(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayRollCash") {
		market->setDayRollCashConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="DayRollSwap"){
		market->setDayRollSwapConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="AccrualAdjustCash"){
		market->setAccrualAdjustCashConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="AccrualAdjustSwap"){
		market->setAccrualAdjustSwapConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="BusinessDaysAfterSpot"){
		market->setBusinessDaysAfterSpot(std::stoi(fieldVal));
	}else if (fieldName=="DayCountBond"){
		market->setDayCountBondConvention(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayRollBond")	{
		market->setDayRollBondConvention(EnumHelper::getDayRollEnum(fieldVal));
	}
}