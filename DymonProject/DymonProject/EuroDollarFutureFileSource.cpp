//created by Wang Jianwei on 1 Dec 2012

#include "EuroDollarFutureFileSource.h"
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

void EuroDollarFutureFileSource::init(Configuration* cfg){
   _name = "EuroDollarFuture";
	_fileName = cfg->getProperty("euroDollarFuture.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("euroDollarFuture.enabled",true,"")=="true"?true:false;
	_monthBeforeDay = cfg->getProperty("euroDollarFuture.monthBeforeDay",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void EuroDollarFutureFileSource::retrieveRecord(){
	if (!_enabled) return;	

	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::EuroDollarFutureMap* Map = RecordHelper::getInstance()->getEuroDollarFutureMap();

	for (int i=1;i<numOfRows;i++) {
		EuroDollarFuture* tempFuture = createFutureObject(db, i);
		tempFuture->genereateReset();
		insertFutureIntoCache(tempFuture, Map);
	}
}

void EuroDollarFutureFileSource::insertFutureIntoCache(EuroDollarFuture* deposit, RecordHelper::EuroDollarFutureMap* euroDollarFutureMap){	
	enums::CurrencyEnum market = deposit->getMarket().getCurrencyEnum();
	long accrualEndJDN = deposit->getExpiryDate().getJudianDayNumber();
	if (euroDollarFutureMap->find(market) == euroDollarFutureMap->end()){
		auto tempMap = map<long, EuroDollarFuture>();
		tempMap.insert(std::make_pair(accrualEndJDN, *deposit));
		euroDollarFutureMap->insert(std::make_pair(market, tempMap));
	}else{
		auto tempMap = &(euroDollarFutureMap->find(market)->second);
		tempMap->insert(std::make_pair(accrualEndJDN, *deposit));
	}
	//cout<<deposit->toString()<<endl;
}

EuroDollarFuture* EuroDollarFutureFileSource::createFutureObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	EuroDollarFuture* tempFuture = new EuroDollarFuture();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateFutureObjectField(fieldName, fieldVal, tempFuture);
	}		
	return tempFuture;
}

void EuroDollarFutureFileSource::updateFutureObjectField(std::string fieldName, std::string fieldVal, EuroDollarFuture* euroDollarFuture){
	if(fieldName=="ID"){
		euroDollarFuture->setID(fieldVal);
	}else if (fieldName=="NAME"){
		euroDollarFuture->setName(fieldVal);
		euroDollarFuture->setTenor(3);
	}else if (fieldName=="PX_MID"){
		euroDollarFuture->setRate(stod(fieldVal));
		euroDollarFuture->setForwardLiborRate(1 - stod(fieldVal)/100);
	}else if (fieldName=="INT_RT_DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		euroDollarFuture->setDayCount(dayCount);
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,_monthBeforeDay);
		euroDollarFuture->setTradeDate(tradeDate);
	} else if (fieldName=="COUNTRY"){
		Market market = Market(EnumHelper::getCcyEnum(fieldVal));
		euroDollarFuture->setMarket(market);
	}else if (fieldName=="SETTLE_DT"){
		date spotDate(fieldVal,_monthBeforeDay);
		euroDollarFuture->setSpotDate(spotDate);
	}else if (fieldName=="INT_RATE_FUT_START_DT"){
		date accrualStartDate(fieldVal,_monthBeforeDay);
		euroDollarFuture->setStartDate(accrualStartDate);
	}else if (fieldName=="FUT_DLV_DT_FIRST"){
		date deliveryDate(fieldVal,_monthBeforeDay);
		euroDollarFuture->setDeliveryDate(deliveryDate);
	}else if (fieldName=="INT_RATE_FUT_END_DT"){
		date expiryDate(fieldVal,_monthBeforeDay);
		euroDollarFuture->setExpiryDate(expiryDate);
	}else if (fieldName=="CONV_ADJ"){
		euroDollarFuture->setConvAdj(stod(fieldVal)/100);
	}
}