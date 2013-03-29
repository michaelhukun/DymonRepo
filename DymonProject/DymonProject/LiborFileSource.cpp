//created by Wang Jianwei on 1 Dec 2012

#include "LiborFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "RecordHelper.h"
#include "Market.h"
#include "EnumHelper.h"

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;
using namespace instruments;

void LiborFileSource::init(Configuration* cfg){
   _name = "Libor";
	_fileName = cfg->getProperty("liborRate.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("liborRate.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void LiborFileSource::retrieveRecord(){
	if (!_enabled) return;	

	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::DepositRateMap* liborRateMap = RecordHelper::getInstance()->getLiborRateMap();

	for (int i=1;i<numOfRows;i++) {
		Libor* tempLibor = createLiborObject(db, i);
		tempLibor->deriveDates();
		insertLiborIntoCache(tempLibor, liborRateMap);
	}
}

void LiborFileSource::insertLiborIntoCache(Libor* libor, RecordHelper::DepositRateMap* liborRateMap){	
	enums::CurrencyEnum market = libor->getCountry();
	long accrualEndJDN = libor->getExpiryDate().getJudianDayNumber();
	if (liborRateMap->find(market) == liborRateMap->end()){
		auto tempMap = map<long, Deposit>();
		tempMap.insert(std::make_pair(accrualEndJDN, *libor));
		liborRateMap->insert(std::make_pair(market, tempMap));
	}else{
		auto tempMap = &(liborRateMap->find(market)->second);
		tempMap->insert(std::make_pair(accrualEndJDN, *libor));
	}
	//cout<<libor->toString()<<endl;
}

Libor* LiborFileSource::createLiborObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	Libor* tempLibor = new Libor();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		try{
			updateLiborObjectField(fieldName, fieldVal, tempLibor);
		}catch(...){
			cout<<"Libor Object Field Error: Name["<<fieldName<<"] Value["<<fieldVal<<"]"<<endl;
		}
	}		
	return tempLibor;
}

void LiborFileSource::updateLiborObjectField(std::string fieldName, std::string fieldVal, Libor* libor){
	if(fieldName=="ID"){
		libor->setID(fieldVal);
	}else if (fieldName=="SECURITY_NAME"){
		libor->setName(fieldVal);
		if (fieldVal.find("/N")!=std::string::npos) 
			libor->setIsOverNight(true);
		else
			libor->setIsOverNight(false);
	}else if (fieldName=="INDX_SOURCE_NAME"){
		string tenorStr = fieldVal.substr(4,fieldVal.length()-4);
		libor->setTenorStr(tenorStr);
		deriveTenor(tenorStr, libor);
	}else if (fieldName=="PX_LAST"){
		libor->setLiborRate(stod(fieldVal)/100);
	}else if (fieldName=="DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		libor->setDayCount(dayCount);
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,false);
		libor->setTradeDate(tradeDate);
		libor->setFixingDate(tradeDate);
	} else if (fieldName=="COUNTRY"){
		Market market = Market(EnumHelper::getCcyEnum(fieldVal));
		libor->setMarket(market);
	} else if (fieldName=="COUNTRY_LABEL"){
		libor->setCountry(EnumHelper::getCcyEnum(fieldVal));
	}
}

void LiborFileSource::deriveTenor(std::string tenorStr, Libor* libor){
	if (tenorStr.find("/N")!=std::string::npos) {
		libor->setDateUnit(dateUtil::getDateUnit(tenorStr));
	}else{
		string dateUnit = tenorStr.substr(tenorStr.size()-1,1);
		string tenorNum = tenorStr.substr(0,tenorStr.size()-1);
		libor->setDateUnit(dateUtil::getDateUnit(dateUnit));
		libor->setTenorNum(stoi(tenorNum));
	}
}