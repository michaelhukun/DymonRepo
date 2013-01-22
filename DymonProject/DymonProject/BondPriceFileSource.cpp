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
	_fileName = cfg->getProperty("bondPrice.file",true,"");
	_persistDir = cfg->getProperty("bondPrice.path",false,"");
	AbstractFileSource::init(cfg);
}

void BondPriceFileSource::retrieveRecord(){
	AbstractFileSource::retrieveRecord();

	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();
	int bondTenorNumOfMonths=0;

	RecordHelper::BondRateMap* bondRateMap = RecordHelper::getInstance()->getBondRateMap();

	for (int i=1;i<numOfRows;i++) {
		Bond* tempBond = createBondObject(db, i);
		insertBondIntoCache(tempBond, bondRateMap);
	}

	_inFile.close();
}

void BondPriceFileSource::insertBondIntoCache(Bond* bond, RecordHelper::BondRateMap* bondRateMap){
	MarketEnum market = bond->getMarket().getMarketEnum();
	long maturityDateJDN = bond->getMaturityDate().getJudianDayNumber();
	if (bondRateMap->find(market) == bondRateMap->end()){
		std::map<long, Bond> tempMap = std::map<long, Bond>();
		tempMap.insert(std::make_pair(maturityDateJDN, *bond));
		bondRateMap->insert(std::make_pair(market, tempMap));
	}else{
		std::map<long, Bond>* tempMap = &(bondRateMap->find(market)->second);
		if (tempMap->find(maturityDateJDN) == tempMap->end()){
			tempMap->insert(std::make_pair(maturityDateJDN, *bond));
		}else{
			 Bond* duplicateBondInMap = &(tempMap->find(maturityDateJDN)->second);
			 duplicateBondInMap->setType("Gen");
		}
	}
	cout<<bond->getShortCut()<<endl;
}

Bond* BondPriceFileSource::createBondObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	Bond* tempBond = new Bond();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateMarketObjectField(fieldName, fieldVal, tempBond);
	}		
	tempBond->setTradeDate(dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following, tempBond->getMarket().getMarketEnum()));	
	tempBond->generateCouponLeg();
	tempBond->deriveDirtyPrice();
	return tempBond;
}

void BondPriceFileSource::updateMarketObjectField(std::string fieldName, std::string fieldVal, Bond* bond){
	if (fieldName=="Cntry") {		
		Market market(EnumHelper::getCcyEnum(fieldVal));
		bond->setMarket(market);
	}else if(fieldName=="Name"){
		bond->setName(fieldVal);
	}else if (fieldName=="Type"){
		bond->setType(fieldVal);
	}else if (fieldName=="Shortcut"){
		bond->setShortCut(fieldVal);
	}else if (fieldName=="Collateral Type"){
		bond->setCollateralType(fieldVal);
	}else if (fieldName=="Bmk"){
		std::regex YR("[0-9]+Y");
		if (fieldVal.find("Y")!=fieldVal.npos) {
			bond->setTenor(std::stoi(fieldVal.substr(0,fieldVal.find("Y")))*12);
		} else if (fieldVal.find("M")!=fieldVal.npos){
			bond->setTenor(std::stoi(fieldVal.substr(0,fieldVal.find("M"))));
		} else {
			bond->setTenor(NaN);
		}
	}else if (fieldName=="CPN"){
		double couponRate = NaN;
		if (fieldVal!="n/a") couponRate = (std::stod(fieldVal)/bond->getCouponFreq())/100;
		bond->setCouponRate(couponRate);
	}else if (fieldName=="CPN_FREQ"){
		int couponFreq=(fieldVal=="n/a")?(int)NaN:std::stoi(fieldVal);
		bond->setCouponFreq(couponFreq);
	}else if (fieldName=="Maturity"){
		date maturityDate(fieldVal,true);
		bond->setMaturityDate(maturityDate);
	}else if (fieldName=="first_cpn_DT"){
		date firstCouponDate(fieldVal,true);
		bond->setFirstCouponDate(firstCouponDate);
	}else if (fieldName=="ISSUE_DT"){
		date issueDate(fieldVal,true);
		bond->setIssueDate(issueDate);
	}else if (fieldName=="PX_MID"){
		double cleanPrice = std::stod(fieldVal);
		bond->setCleanPrice(cleanPrice);
	}else if (fieldName=="DAY_CNT_DES"){
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(fieldVal);
		bond->setDayCount(dayCount);
	}
}
