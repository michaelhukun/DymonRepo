//implemented by Kun from bloomberg market data sources on 13 Jan 2013
//Revamped by Jianwei
#include "BondPriceFileSource.h"
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

using namespace DAO;
void BondPriceFileSource::init(Configuration* cfg){
   _name = "Bond";
	_fileName = cfg->getProperty("bondPrice.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("bondPrice.enabled",true,"")=="true"?true:false;
	_monthBeforeDay = cfg->getProperty("bondPrice.monthBeforeDay",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void BondPriceFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();
	int bondTenorNumOfMonths=0;

	RecordHelper::BondRateMap* bondRateMap = RecordHelper::getInstance()->getBondRateMap();

	for (int i=1;i<numOfRows;i++) {
		Bond* tempBond = createBondObject(db, i);
		insertBondIntoCache(tempBond, bondRateMap);
	}
}

void BondPriceFileSource::insertBondIntoCache(Bond* bond, RecordHelper::BondRateMap* bondRateMap){
	CurrencyEnum market = bond->getMarket().getCurrencyEnum();
	long maturityDateJDN = bond->getDeliveryDate().getJudianDayNumber();
	if (bondRateMap->find(market) == bondRateMap->end()){
		std::map<long, Bond> tempMap = std::map<long, Bond>();
		tempMap.insert(std::make_pair(maturityDateJDN, *bond));
		bondRateMap->insert(std::make_pair(market, tempMap));
	}else{
		std::map<long, Bond>* tempMap = &(bondRateMap->find(market)->second);
		tempMap->insert(std::make_pair(maturityDateJDN, *bond));
	}
	//cout<<bond->toString()<<endl;
}

Bond* BondPriceFileSource::createBondObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	Bond* tempBond = new Bond();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateMarketObjectField(fieldName, fieldVal, tempBond);
	}		
	tempBond->setNotional(100);
	tempBond->deriveDates();
	tempBond->generateCouponLeg();
	tempBond->deriveDirtyPrice();
	return tempBond;
}

void BondPriceFileSource::updateMarketObjectField(std::string fieldName, std::string fieldVal, Bond* bond){
	if (fieldName=="COUNTRY") {		
		Market market(EnumHelper::getCcyEnum(fieldVal));
		bond->setMarket(market);
		bond->setDayRoll(market.getDayRollBondConvention());
	}else if(fieldName=="NAME"){
		bond->setName(fieldVal);
	}else if(fieldName=="ID"){
		bond->setID(fieldVal);
	}else if (fieldName=="ID_CUSIP"){
		bond->setCUSIP(fieldVal);
	}else if (fieldName=="SECURITY_TYP2"){
		bond->setSecurityType(fieldVal);
	}else if (fieldName=="YRS_TO_MTY_ISSUE"){
		int tenorInYear = (int)(std::stod(fieldVal)+0.01);
		bond->setTenor(tenorInYear);
	}else if (fieldName=="CPN"){
		double couponRate = 0;
		if (fieldVal.find("#N/A")==std::string::npos) couponRate = (std::stod(fieldVal)/bond->getCouponFreq())/100;
		bond->setCouponRate(couponRate);
	}else if (fieldName=="CPN_FREQ"){
		int couponFreq=(fieldVal.find("#N/A")!=std::string::npos)?1:std::stoi(fieldVal);
		bond->setCouponFreq(couponFreq);
	}else if (fieldName=="MATURITY"){
		date maturityDate(fieldVal, _monthBeforeDay);
		bond->setExpiryDate(maturityDate);
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,_monthBeforeDay);
		bond->setTradeDate(tradeDate);
	}else if (fieldName=="FIRST_CPN_DT"){
		date firstCouponDate(fieldVal,_monthBeforeDay);
		bond->setFirstCouponDate(firstCouponDate);
	}else if (fieldName=="PREV_CPN_DT"){
		date prevCouponDate(fieldVal,_monthBeforeDay);
		bond->setPrevCouponDate(prevCouponDate);
	}else if (fieldName=="NXT_CPN_DT"){
		date nextCouponDate(fieldVal,_monthBeforeDay);
		bond->setNextCouponDate(nextCouponDate);
	}else if (fieldName=="ISSUE_DT"){
		date issueDate(fieldVal,_monthBeforeDay);
		bond->setIssueDate(issueDate);
	}else if (fieldName=="PX_MID"){
		double cleanPrice = std::stod(fieldVal);
		bond->setCleanPrice(cleanPrice);
	}else if (fieldName=="YAS_ISPREAD_TO_GOVT"){
		if (std::string::npos != fieldVal.find("N/A"))	{
			bond->setQuotedGSpread(NaN);
		}else{
			bond->setQuotedGSpread(std::stod(fieldVal));
		}
	}else if (fieldName=="YLD_YTM_MID"){
		double quotedYTM = std::stod(fieldVal);
		bond->setQuotedYTM(quotedYTM);
	}else if (fieldName=="DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		bond->setDayCount(dayCount);
	}else if (fieldName=="IS_US_ON_THE_RUN"){
		bool isGeneric = (fieldVal=="Y")?true:false;
		bond->setIsGeneric(isGeneric);
	}else if (fieldName=="TRADING_DT_REALTIME"){
		date tradeDate(fieldVal,_monthBeforeDay);
		bond->setTradeDate(tradeDate);
	}else if (fieldName=="SETTLE_DT"){
		date accrualStartDate(fieldVal,_monthBeforeDay);
		bond->setSpotDate(accrualStartDate);
	}
}
