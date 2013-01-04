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

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;
using namespace instruments;

SwapRateFileSource::SwapRateFileSource():
	AbstractFileSource(){}

SwapRateFileSource::SwapRateFileSource(std::string persistDir, std::string fileName):
	AbstractFileSource(persistDir, fileName){}

SwapRateFileSource::~SwapRateFileSource(){}

void SwapRateFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("swapRate.file",true,"");
	_persistDir = cfg->getProperty("swapRate.path",false,"");
	AbstractFileSource::init(cfg);
}

void SwapRateFileSource::retrieveRecord(){
	AbstractFileSource::retrieveRecord();
	
	string value;
	enums::MarketEnum market;
	RecordHelper::RateMap tempMap;
	while (_inFile.good()){
		_inFile>>value;
		vector<string> vec = fileUtil::split(value,':');
		market = EnumHelper::getCcyEnum(vec[0]);
		Market mkt(market);
		enums::DayRollEnum accrualAdjust=mkt.getAccrualAdjustSwapConvention();
		int businessDaysAfterSpot = mkt.getBusinessDaysAfterSpot();
		vector<string> deposits = fileUtil::split(vec[1],',');
		cout<<market<<" market has total swap rate number:  "<<deposits.size()<<endl;

		std::map<long, double> rateMap;
		for (unsigned int i = 0; i<deposits.size(); i++)
		{
			// 2Y-3.134
			date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,market);	
			date accrualStartDate = dateUtil::getBizDateOffSet(startDate,mkt.getBusinessDaysAfterSpot(),market); // day after spot adjust
			vector<string> tenureRate = fileUtil::split(deposits[i],'=');
			char letterDateUnit = *tenureRate[0].rbegin(); // 'Y'
			int increment = std::stoi(tenureRate[0].substr(0,tenureRate[0].size()-1)); // 2
			double swapRate = std::stod(tenureRate[1])/100.0; // 3.134
			long accrualEndJDN = dateUtil::getEndDate(accrualStartDate,increment, accrualAdjust, market, dateUtil::getDateUnit(letterDateUnit)).getJudianDayNumber();
			rateMap.insert(pair<long, double>(accrualEndJDN, swapRate));
			
			date accrualEndDate(accrualEndJDN);
			cout << mkt.getNameString()<< " -> tenor[" << tenureRate[0]<<"], accrual start["<<accrualStartDate.toString()<<"], accrual end["
				<<accrualEndDate.toString() <<"], deposit rate["<< swapRate << "]"<<endl;
		}
		tempMap.insert(pair<enums::MarketEnum, map<long, double>>(market,rateMap));
	}
	RecordHelper::getInstance()->setSwapRateMap(tempMap);
	_inFile.close();
}
