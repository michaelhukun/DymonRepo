#include "BondFutureFileSource.h"
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

void BondFutureFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("bondFuture.file",true,"");
	_persistDir = cfg->getProperty("bondFuture.path",false,"");
	_enabled = cfg->getProperty("bondFuture.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void BondFutureFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	AbstractFileSource::retrieveRecord();
	CSVDatabase db;
	readCSV(_inFile, db);
	int numOfRows=db.size();
	RecordHelper::BondFutureMap* bondRateMap = RecordHelper::getInstance()->getBondFutureMap();

	for (int i=1;i<numOfRows;i++) {
		BondFuture* tempBondFuture = createBondFutureObject(db, i);
		insertBondFutureIntoCache(tempBondFuture, bondRateMap);
	}

	_inFile.close();
}

void BondFutureFileSource::insertBondFutureIntoCache(BondFuture* bondFuture, RecordHelper::BondFutureMap* bondFutureMap){
	CurrencyEnum market = bondFuture->getMarket().getCurrencyEnum();
	if (bondFutureMap->find(market) == bondFutureMap->end()){
		std::map<long, BondFuture> tempMap = std::map<long, BondFuture>();
		tempMap.insert(std::make_pair(bondFuture->getNotionalBondTerm(), *bondFuture));
		bondFutureMap->insert(std::make_pair(market, tempMap));
	}else{
		std::map<long, BondFuture>* tempMap = &(bondFutureMap->find(market)->second);
		tempMap->insert(std::make_pair(bondFuture->getNotionalBondTerm(), *bondFuture));
	}
	cout<<bondFuture->getName()<<endl;
}

BondFuture* BondFutureFileSource::createBondFutureObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	BondFuture* tempBondFuture = new BondFuture();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateBondFutureField(fieldName, fieldVal, tempBondFuture);
	}		
	tempBondFuture->setTradeDate(dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following, tempBondFuture->getMarket().getCurrencyEnum()));	
	tempBondFuture->setCTDBond(RecordHelper::getInstance()->findCTDinBondMap(tempBondFuture->getCTDCUSIP()));
	return tempBondFuture;
}

void BondFutureFileSource::updateBondFutureField(std::string fieldName, std::string fieldVal, BondFuture* bondFuture){
	if (fieldName=="COUNTRY") {		
		Market market(EnumHelper::getCcyEnum(fieldVal));
		bondFuture->setMarket(market);
	}else if(fieldName=="ID"){
		bondFuture->setID(fieldVal);
	}else if(fieldName=="NAME"){
		bondFuture->setName(fieldVal);
	}else if (fieldName=="FUT_CONT_SIZE"){
		bondFuture->setContractSize(std::stoi(fieldVal));
	}else if (fieldName=="PX_MID"){
		bondFuture->setQuotedPrice(std::stod(fieldVal));
	}else if (fieldName=="NOTIONAL_ISSUE_TERM_NUMBER"){
		bondFuture->setNotionalBondTerm(std::stoi(fieldVal));
	}else if (fieldName=="NOTIONAL_ISSUE_COUPON"){
		bondFuture->setNotionalBondCouponRate(std::stod(fieldVal)/100);
	}else if (fieldName=="FUT_CTD_CUSIP"){
		bondFuture->setCTDCUSIP(fieldVal);
	}else if (fieldName=="FUT_CTD_CONVERTION"){
		bondFuture->setCTDConversionFactor(std::stod(fieldVal));
	}else if (fieldName=="FUT_FIRST_TRADE_DT"){
		date firstTradeDate(fieldVal,true);
		bondFuture->setIssueDate(firstTradeDate);
		bondFuture->setFirstTradeDate(firstTradeDate);
	}else if (fieldName=="LAST_TRADEABLE_DT"){
		date lastTradeDate(fieldVal,true);
		bondFuture->setMaturityDate(lastTradeDate);
		bondFuture->setLastTradeDate(lastTradeDate);
	}else if (fieldName=="FUT_DLV_DT_FIRST"){
		date firstDeliveryDate(fieldVal,true);
		bondFuture->setFirstDeliverDate(firstDeliveryDate);
	}else if (fieldName=="FUT_DLV_DT_LAST"){
		date lastDeliveryDate(fieldVal,true);
		bondFuture->setLastDeliverDate(lastDeliveryDate);
	}else if (fieldName=="DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		bondFuture->setDayCount(dayCount);
	}
}
