//created by Wang Jianwei on 1 Dec 2012

#include "DepositFileSource.h"
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

void DepositFileSource::init(Configuration* cfg){
   _name = "Deposit";
	_fileName = cfg->getProperty("depositRate.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("depositRate.enabled",true,"")=="true"?true:false;
	_monthBeforeDay = cfg->getProperty("depositRate.monthBeforeDay",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void DepositFileSource::retrieveRecord(){
	if (!_enabled) return;	

	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::DepositRateMap* depositRateMap = RecordHelper::getInstance()->getDepositRateMap();

	for (int i=1;i<numOfRows;i++) {
		Deposit* tempDeposit = createDepositObject(db, i);
		tempDeposit->deriveAccrualStartDate();
		insertDepositIntoCache(tempDeposit, depositRateMap);
	}
}

void DepositFileSource::insertDepositIntoCache(Deposit* deposit, RecordHelper::DepositRateMap* depositRateMap){	
	enums::CurrencyEnum market = deposit->getMarket().getCurrencyEnum();
	long accrualEndJDN = deposit->getExpiryDate().getJudianDayNumber();
	if (depositRateMap->find(market) == depositRateMap->end()){
		auto tempMap = map<long, Deposit>();
		tempMap.insert(std::make_pair(accrualEndJDN, *deposit));
		depositRateMap->insert(std::make_pair(market, tempMap));
	}else{
		auto tempMap = &(depositRateMap->find(market)->second);
		tempMap->insert(std::make_pair(accrualEndJDN, *deposit));
	}
	//cout<<deposit->toString()<<endl;
}

Deposit* DepositFileSource::createDepositObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	Deposit* tempDeposit = new Deposit();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateDepositObjectField(fieldName, fieldVal, tempDeposit);
	}		
	return tempDeposit;
}

void DepositFileSource::updateDepositObjectField(std::string fieldName, std::string fieldVal, Deposit* deposit){
	if(fieldName=="ID"){
		deposit->setID(fieldVal);
	}else if (fieldName=="NAME"){
		deposit->setName(fieldVal);
		if (fieldVal.find("/N")!=std::string::npos) 
			deposit->setIsOverNight(true);
		else
			deposit->setIsOverNight(false);
	}else if (fieldName=="SECURITY_TENOR_ONE"){
		deposit->setTenorStr(fieldVal);
	}else if (fieldName=="PX_MID"){
		deposit->setDepositRate(stod(fieldVal)/100);
	}else if (fieldName=="DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		deposit->setDayCount(dayCount);
	}else if (fieldName=="DAYS_TO_MTY"){
		deposit->setDaysToMty(stoi(fieldVal));
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,_monthBeforeDay);
		deposit->setTradeDate(tradeDate);
	} else if (fieldName=="COUNTRY"){
		Market market = Market(EnumHelper::getCcyEnum(fieldVal));
		deposit->setMarket(market);
	}else if (fieldName=="SETTLE_DT"){
		date deliveryDate(fieldVal,_monthBeforeDay);
		deposit->setDeliveryDate(deliveryDate);
	}else if (fieldName=="MATURITY"){
		date accrualEndDate(fieldVal,_monthBeforeDay);
		deposit->setExpiryDate(accrualEndDate);
	}
}