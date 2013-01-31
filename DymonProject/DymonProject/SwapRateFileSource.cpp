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
	_enabled = cfg->getProperty("swapRate.enabled",true,"")=="True"?true:false;
	AbstractFileSource::init(cfg);
}

void SwapRateFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	AbstractFileSource::retrieveRecord();
	enums::CurrencyEnum CurrencyEnum;
	RecordHelper::RateMap swapRateMap;

	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	for (int i=1;i<numOfCols;i++) {

		CurrencyEnum=EnumHelper::getCcyEnum(db.at(0).at(i));
		Market market = Market(CurrencyEnum);

		map<long, double>* tempMap = new map<long, double>;

		for (int j = 1; j<numOfRows; j++)
		{
			string tenorStr = db.at(j).at(0);
			double liborRate = std::stod(db.at(j).at(i))/100.0;
			insertRateIntoMap(tenorStr, liborRate, market, tempMap);
		}
		swapRateMap.insert(pair<enums::CurrencyEnum, map<long, double>>(CurrencyEnum,*tempMap));
	}

	RecordHelper::getInstance()->setSwapRateMap(swapRateMap);
	_inFile.close();
}

void SwapRateFileSource::insertRateIntoMap(std::string tenorStr, double swapRate, Market market, std::map<long, double>* rateMap){
	date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following, market.getCurrencyEnum());	
	date accrualStartDate = dateUtil::getBizDateOffSet(startDate,market.getBusinessDaysAfterSpot(enums::SWAP), market.getCurrencyEnum()); // day after spot adjust
	char tenorUnit = *tenorStr.rbegin(); // 'Y'
	int tenorNum = std::stoi(tenorStr.substr(0,tenorStr.size()-1)); // 2
	long accrualEndJDN = dateUtil::getEndDate(accrualStartDate,tenorNum, market.getDayRollSwapConvention(), market.getCurrencyEnum(), dateUtil::getDateUnit(tenorUnit)).getJudianDayNumber();
	rateMap->insert(pair<long, double>(accrualEndJDN, swapRate));

	date accrualEndDate(accrualEndJDN);
	cout << market.getNameString()<< " -> tenor[" << tenorStr<<"], accrual start["<<accrualStartDate.toString()<<"], accrual end["
		<<accrualEndDate.toString() <<"], deposit rate["<< swapRate << "]"<<endl;
}