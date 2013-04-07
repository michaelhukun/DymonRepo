//created by Wang Jianwei on 1 Dec 2012

#include "SwapRateFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "RecordHelper.h"
#include "Enums.h"
#include "EnumHelper.h"
#include "Market.h"
#include "Swap.h"

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;
using namespace instruments;

void SwapRateFileSource::init(Configuration* cfg){
   _name = "Swap Rate";
	_fileName = cfg->getProperty("swapRate.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("swapRate.enabled",true,"")=="true"?true:false;
	_monthBeforeDay = cfg->getProperty("swapRate.monthBeforeDay",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void SwapRateFileSource::retrieveRecord(){
	if (!_enabled) return;	

	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::SwapRateMap* swapRateMap = RecordHelper::getInstance()->getSwapRateMap();

	for (int i=1;i<numOfRows;i++) {
		Swap* tempSwap = createSwapObject(db, i);
		tempSwap->setNotional(1000000);
		tempSwap->deriveDates();
		tempSwap->deriveDayCount();
		tempSwap->buildFixedLeg();
		tempSwap->buildFloatLeg();
		insertSwapIntoCache(tempSwap, swapRateMap);
	}
}

void SwapRateFileSource::insertSwapIntoCache(Swap* swap, RecordHelper::SwapRateMap* swapRateMap){
	enums::CurrencyEnum market = swap->getMarket().getCurrencyEnum();
	long accrualEndJDN = swap->getExpiryDate().getJudianDayNumber();
	if (swapRateMap->find(market) == swapRateMap->end()){
		auto tempMap = map<long, Swap>();
		tempMap.insert(std::make_pair(accrualEndJDN, *swap));
		swapRateMap->insert(std::make_pair(market, tempMap));
	}else{
		auto tempMap = &(swapRateMap->find(market)->second);
		tempMap->insert(std::make_pair(accrualEndJDN, *swap));
	}
	//cout<<swap->toString()<<endl;
}

Swap* SwapRateFileSource::createSwapObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	Swap* tempSwap = new Swap();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateSwapObjectField(fieldName, fieldVal, tempSwap);
	}		
	return tempSwap;
}

void SwapRateFileSource::updateSwapObjectField(std::string fieldName, std::string fieldVal, Swap* swap){
	if(fieldName=="ID"){
		swap->setID(fieldVal);
	}else if (fieldName=="NAME"){
		swap->setName(fieldVal);
	}else if (fieldName=="SECURITY_TENOR_TWO"){
		swap->setTenorStr(fieldVal);
		swap->setTenorInYear(stoi(fieldVal.substr(0, fieldVal.size()-1)));
	}else if (fieldName=="PX_MID"){
		swap->setSwapRate(stod(fieldVal)/100);
	}else if (fieldName=="DAYS_TO_MTY"){
		swap->setDaysToMty(stoi(fieldVal));
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,_monthBeforeDay);
		swap->setTradeDate(tradeDate);
		swap->setIssueDate(tradeDate);
	}else if (fieldName=="SETTLE_DT"){
		date accrualStartDate(fieldVal,_monthBeforeDay);
		swap->setSpotDate(accrualStartDate);
		swap->setStartDate(accrualStartDate);
	} else if (fieldName=="COUNTRY"){
		Market market = Market(EnumHelper::getCcyEnum(fieldVal));
		swap->setMarket(market);
		swap->setDayRoll(market.getDayRollSwapConvention());
	}	
}