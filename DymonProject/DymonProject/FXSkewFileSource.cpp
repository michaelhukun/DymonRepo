#include "FXSkewFileSource.h"
#include <regex>
#include "Constants.h"
#include "EnumHelper.h"
#include "FXEuropeanOption.h"

using namespace DAO;
using namespace utilities;

void FXSkewFileSource::init(Configuration* cfg){
   _name = "FX Skew";
	_fileName = cfg->getProperty("FXSkew.file",true,"");
	_persistDir = cfg->getProperty("FXSkew.path",false,"");
	_enabled = cfg->getProperty("FXSkew.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void FXSkewFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	AbstractFileSource::retrieveRecord();
	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::FXVolSkewMap* FXVolSkewMap = RecordHelper::getInstance()->getFXVolSkewMap();

	for (int i=1;i<numOfRows;i++) {
		FXEuropeanOption* tempOption = createOptionObject(db, i);
		tempOption->deriveTenorExpiry();
		insertOptionIntoCache(tempOption, FXVolSkewMap);
	}

	_inFile.close();
}

void FXSkewFileSource::insertOptionIntoCache(FXEuropeanOption* option, RecordHelper::FXVolSkewMap* FXVolSkewMap){
	
	string ccyPairStr = option->getCcyPair()->getCcyPairStr();
	if (FXVolSkewMap->find(ccyPairStr) == FXVolSkewMap->end()){
		auto tempMap = map<int, vector<FXEuropeanOption>>();
		FXVolSkewMap->insert(std::make_pair(ccyPairStr, tempMap));
	}

	auto* tempMap = &(FXVolSkewMap->find(ccyPairStr)->second);

	int daysToExpiry = option->getDaysToExpiry();
	if (tempMap->find(daysToExpiry) == tempMap->end()){
		auto tempVector = vector<FXEuropeanOption>();
		tempMap->insert(std::make_pair(daysToExpiry, tempVector));
	}

	auto tempVector = &(tempMap->find(daysToExpiry)->second);
	tempVector->push_back(*option);
}

FXEuropeanOption* FXSkewFileSource::createOptionObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	FXEuropeanOption* tempOption = new FXEuropeanOption();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateOptionObjectField(fieldName, fieldVal, tempOption);
	}		
	return tempOption;
}

void FXSkewFileSource::updateOptionObjectField(std::string fieldName, std::string fieldVal, FXEuropeanOption* option){
	if(fieldName=="ID"){
		option->setID(fieldVal);
	}else if (fieldName=="SECURITY_TENOR_ONE"){
		option->setTenorStr(fieldVal);
	}else if (fieldName=="PX_MID"){
		option->setVol(stod(fieldVal));
	}else if (fieldName=="VOL_INSTRUMENT_TYPE"){
		option->setVolType(EnumHelper::getVolType(fieldVal));
	}else if (fieldName=="VOL_DELTA"){
		if (fieldVal == "#N/A Field Not Applicable")
			option->setDelta(NaN);
		else
			option->setDelta(stod(fieldVal.substr(0, fieldVal.size()-1))/100);
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,false);
		option->setTradeDate(tradeDate);
		option->setSpotDate(tradeDate);
	}else if (fieldName=="SETTLE_DT"){
		date settleDate(fieldVal,false);
		option->setDeliveryDate(settleDate);
	}else if (fieldName=="OPT_EXPIRE_DT"){
		date expiryDate(fieldVal,false);
		option->setExpiryDate(expiryDate);
	}else if (fieldName=="VOL_CURRENCY_ISO_PAIR"){
		option->setCcyPair(CcyPair(fieldVal));
	}else if (fieldName=="OPT_DAYS_EXPIRE"){
		option->setDaysToExpiry(stoi(fieldVal));
	}
}