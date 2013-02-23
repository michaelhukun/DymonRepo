#include "FXForwardFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "RecordHelper.h"
#include "Enums.h"
#include "EnumHelper.h"
#include "Market.h"
#include <tuple>
#include <regex>
#include "Constants.h"

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;
using namespace instruments;

void FXForwardFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("FXForward.file",true,"");
	_persistDir = cfg->getProperty("FXForward.path",false,"");
	_enabled = cfg->getProperty("FXForward.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void FXForwardFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	AbstractFileSource::retrieveRecord();
	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::FXForwardMap* FXForwardMap = RecordHelper::getInstance()->getFXForwardMap();

	for (int i=1;i<numOfRows;i++) {
		FXForward* tempForward = createForwardObject(db, i);
		insertForwardIntoCache(tempForward, FXForwardMap);
	}
	deriveSpotForwardRate(FXForwardMap);

	_inFile.close();
}

void FXForwardFileSource::insertForwardIntoCache(FXForward* forward, RecordHelper::FXForwardMap* FXForwardMap){
	string ccyPairStr = forward->getCcyPair()->getCcyPairStr();
	long deliveryDateJDN = forward->getDeliveryDate().getJudianDayNumber();
	if (FXForwardMap->find(ccyPairStr) == FXForwardMap->end()){
		auto tempMap = map<long, FXForward>();
		tempMap.insert(std::make_pair(deliveryDateJDN, *forward));
		FXForwardMap->insert(std::make_pair(ccyPairStr, tempMap));
	}else{
		auto tempMap = &(FXForwardMap->find(ccyPairStr)->second);
		tempMap->insert(std::make_pair(deliveryDateJDN, *forward));
	}
	cout<<forward->toString()<<endl;
}

FXForward* FXForwardFileSource::createForwardObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	FXForward* tempForward = new FXForward();
	tempForward->setCcyPair(CcyPair());

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateObjectField(fieldName, fieldVal, tempForward);
	}		
	return tempForward;
}

void FXForwardFileSource::updateObjectField(std::string fieldName, std::string fieldVal, FXForward* forward){
	if(fieldName=="ID"){
		forward->setID(fieldVal);
	}else if (fieldName=="SECURITY_TENOR_ONE"){
		forward->setTenorStr(fieldVal);
	}else if (fieldName=="PX_MID"){
		std::regex IDRegex ("[A-Z]{3} BGN Curncy");
		if (std::regex_match (forward->getID(),IDRegex)){
			forward->setSpot(std::stod(fieldVal));
			forward->setIsSpot(true);
		} else{
			forward->setPoint(std::stod(fieldVal));
			forward->setIsSpot(false);
		}
	}else if (fieldName=="DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		forward->setDayCount(dayCount);
	}else if (fieldName=="DAYS_TO_MTY"){
		forward->setDaysToMty(stoi(fieldVal));
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,false);
		forward->setTradeDate(tradeDate);
	}else if (fieldName=="SETTLE_DT"){
		date settleDate(fieldVal,false);
		forward->setDeliveryDate(settleDate);
	}else if (fieldName=="MATURITY"){
		date expiryDate(fieldVal,false);
		forward->setExpiryDate(expiryDate);
	}else if (fieldName=="CRNCY"){
		forward->getCcyPair()->setCCY1(fieldVal);
	}else if (fieldName=="BASE_CRNCY"){
		forward->getCcyPair()->setCCY2(fieldVal);
	}
}

void FXForwardFileSource::deriveSpotForwardRate(RecordHelper::FXForwardMap* FXForwardMap){
	for (auto it = FXForwardMap->begin(); it!=FXForwardMap->end(); it++){
		double spotRate=NaN;
		date spotDate=date("",true);
		auto* innerMap = &(it->second);
		
		// get spot rate and spot date
		for (auto innerIt = innerMap->begin(); innerIt!=innerMap->end(); innerIt++){
			FXForward* forward = &(innerIt->second);
			if (forward->getIsSpot()){
				spotRate = forward->getSpot();
				spotDate = forward->getDeliveryDate();			
				forward->setPoint(0);
			}
		}

		// set spot rate and derive outright
		for (auto innerIt = innerMap->begin(); innerIt!=innerMap->end(); innerIt++){
			FXForward* forward = &(innerIt->second);
			forward->setSpot(spotRate);
			forward->setSpotDate(spotDate);
			forward->setOutRight(spotRate+forward->getPoint()/10000);
		}
	}
}
