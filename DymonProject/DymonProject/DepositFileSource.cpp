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

DepositFileSource::DepositFileSource():
AbstractFileSource(){}

DepositFileSource::DepositFileSource(std::string persistDir, std::string fileName):
AbstractFileSource(persistDir, fileName){}

DepositFileSource::~DepositFileSource(){}

void DepositFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("depositRate.file",true,"");
	_persistDir = cfg->getProperty("depositRate.path",false,"");
	AbstractFileSource::init(cfg);
}

void DepositFileSource::retrieveRecord(){
	AbstractFileSource::retrieveRecord();

	enums::MarketEnum marketEnum;

	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::RateMap tempDepositMap;
	RecordHelper::RateMap tempOvernightMap;

	for (int i=1;i<numOfCols;i++) {

		marketEnum=EnumHelper::getCcyEnum(db.at(0).at(i));
		Market market = Market(marketEnum);

		map<long, double>* depositRateMap = new map<long, double>;
		map<long, double>* overnightRateMap = new map<long, double>;

		for (int j = 1; j<numOfRows; j++)
		{
			string tenorStr = db.at(j).at(0);
			double liborRate = std::stod(db.at(j).at(i))/100.0;
			insertRateIntoMap(tenorStr, liborRate, market, depositRateMap, overnightRateMap);
		}
		tempDepositMap.insert(pair<enums::MarketEnum, map<long, double>>(marketEnum,*depositRateMap));
		tempOvernightMap.insert(pair<enums::MarketEnum, map<long, double>>(marketEnum,*overnightRateMap));
	}
	RecordHelper::getInstance()->setDepositRateMap(tempDepositMap);
	RecordHelper::getInstance()->setOverNightRateMap(tempOvernightMap);
	_inFile.close();
}

void DepositFileSource::insertRateIntoMap(std::string tenorStr, double liborRate, Market market, map<long, double>* depositRateMap, map<long, double>* overnightRateMap){
	date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,market.getMarketEnum());	
	char tenorUnit = *tenorStr.rbegin();
	int tenorNum = std::stoi(tenorStr.substr(0,tenorStr.size()-1)); // 2
	if (tenorUnit != 'D')
		startDate = dateUtil::getBizDateOffSet(startDate,market.getBusinessDaysAfterSpot(enums::SWAP),market.getMarketEnum()); // day after spot adjust

	long JDN = dateUtil::getEndDate(startDate,tenorNum, market.getDayRollCashConvention(), market.getMarketEnum(), dateUtil::getDateUnit(tenorUnit)).getJudianDayNumber();
	if (tenorUnit == 'D'){
		overnightRateMap->insert(pair<long, double>(tenorNum, liborRate));
		cout << market.getNameString()<< " -> tenor[" << tenorStr <<"], accrual start["<<startDate.toString()<<"], duration ["
			<<tenorNum <<"], deposit rate["<< liborRate << "]"<<endl;
	}else{
		date accrualEndDate(JDN);
		depositRateMap->insert(pair<long, double>(JDN, liborRate));
		cout << market.getNameString()<< " -> tenor[" << tenorStr<<"], accrual start["<<startDate.toString()<<"], accrual end["
			<<accrualEndDate.toString() <<"], deposit rate["<< liborRate << "]"<<endl;
	}
}