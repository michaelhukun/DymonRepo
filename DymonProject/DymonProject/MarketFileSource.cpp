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
   _name = "Market Convention";
	_fileName = cfg->getProperty("marketConvention.file",true,"");
	_persistDir = cfg->getProperty("marketConvention.path",false,"");
	_enabled = cfg->getProperty("marketConvention.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void MarketFileSource::retrieveRecord(){
	if (!_enabled) return;

	CSVDatabase db = readCSV(_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();
	RecordHelper::MarketMap marketMap;

	for (int j=1;j<numOfCols;j++) {

		String marketStr = db.at(0).at(j);
		enums::CurrencyEnum CurrencyEnum = EnumHelper::getCcyEnum(marketStr);
		Market* market = new Market();

		for (int i=0;i<numOfRows;i++){
			String fieldName=db.at(i).at(0);
			String fieldVal=db.at(i).at(j);
			updateMarketObjectField(fieldName, fieldVal, market);
		}
		marketMap.insert(std::make_pair(CurrencyEnum,*market));
	}
	RecordHelper::getInstance()->setMarketMap(marketMap);
}

void MarketFileSource::updateMarketObjectField(std::string fieldName, std::string fieldVal, Market* market){

	if (fieldName=="Market"){
		market->setCurrencyEnum(EnumHelper::getCcyEnum(fieldVal));
	}else if (fieldName=="DayCountCash"){
		market->setDayCountCashConvention(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayCountSwapFixed"){
		market->setDayCountSwapFixed(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayCountSwapFloat"){
		market->setDayCountSwapFloat(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayCountBond"){
		market->setDayCountBondConvention(EnumHelper::getDayCountEnum(fieldVal));
	}else if (fieldName=="DayRollCash") {
		market->setDayRollCashConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="DayRollSwap"){
		market->setDayRollSwapConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="DayRollBond")	{
		market->setDayRollBondConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="AccrualAdjustCash"){
		market->setAccrualAdjustCashConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="AccrualAdjustSwap"){
		market->setAccrualAdjustSwapConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="AccrualAdjustBond"){
		market->setAccrualAdjustBondConvention(EnumHelper::getDayRollEnum(fieldVal));
	}else if (fieldName=="BizDaysAfterSpotSwap"){
		market->setBusinessDaysAfterSpotSwap(std::stoi(fieldVal));
	}else if (fieldName=="BizDaysAfterSpotBond"){
		market->setBusinessDaysAfterSpotBond(std::stoi(fieldVal));
	}else if (fieldName=="PayFreqSwapFixed"){
		market->setPayFreqSwapFixed(std::stoi(fieldVal));
	}else if (fieldName=="PayFreqSwapFloat"){
		market->setPayFreqSwapFloat(std::stoi(fieldVal));
	}
}