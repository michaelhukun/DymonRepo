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

	string value;
	enums::MarketEnum market;
	RecordHelper::RateMap tempDepositMap;
	RecordHelper::RateMap tempOvernightMap;
	while (_inFile.good()){
		_inFile>>value;
		vector<string> vec = fileUtil::split(value,':');
		market = EnumHelper::getCcyEnum(vec[0]);
		Market mkt(market);
		enums::DayRollEnum accrualAdjust=mkt.getAccrualAdjustCashConvention();
		vector<string> deposits = fileUtil::split(vec[1],',');
		cout<<market<<" total deposits number:  "<<deposits.size()<<endl;

		std::map<long, double> depositRateMap;
		std::map<long, double> overnightRateMap;
		for (unsigned int i = 0; i<deposits.size(); i++)
		{
			// 2D-0.1		
			vector<string> tenureRate = fileUtil::split(deposits[i],'=');
			char letterDateUnit = *tenureRate[0].rbegin(); // 'D'
			date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,market);	
			if (letterDateUnit != 'D')
				startDate = dateUtil::getBizDateOffSet(startDate,mkt.getBusinessDaysAfterSpot(),market); // day after spot adjust
			int increment = std::stoi(tenureRate[0].substr(0,tenureRate[0].size()-1)); // 2
			double depositRate = std::stod(tenureRate[1])/100.0; // 0.1
			long JDN = dateUtil::getEndDate(startDate,increment, accrualAdjust, market, dateUtil::getDateUnit(letterDateUnit)).getJudianDayNumber();
			if (letterDateUnit == 'D'){
				overnightRateMap.insert(pair<long, double>(increment, depositRate));
				cout << mkt.getNameString()<< " -> tenor[" << tenureRate[0]<<"], accrual start["<<startDate.toString()<<"], duration ["
					<<increment <<"], deposit rate["<< depositRate << "]"<<endl;
			}else{
				date accrualEndDate(JDN);
				depositRateMap.insert(pair<long, double>(JDN, depositRate));
				cout << mkt.getNameString()<< " -> tenor[" << tenureRate[0]<<"], accrual start["<<startDate.toString()<<"], accrual end["
					<<accrualEndDate.toString() <<"], deposit rate["<< depositRate << "]"<<endl;
			}
		}
		tempDepositMap.insert(pair<enums::MarketEnum, map<long, double>>(market,depositRateMap));
		tempOvernightMap.insert(pair<enums::MarketEnum, map<long, double>>(market,overnightRateMap));
	}
	RecordHelper::getInstance()->setDepositRateMap(tempDepositMap);
	RecordHelper::getInstance()->setOverNightRateMap(tempOvernightMap);
	_inFile.close();
}
