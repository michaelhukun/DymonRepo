//implemented by Kun from bloomberg market data sources on 13 Jan 2013
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
	
	RecordHelper::BondRateMap bondRateMap;
	std::map<long,Bond> temp;
	enums::MarketEnum market;
	std::regex CURR ("[A-Z]{3}");
	std::regex YR("[0-9]+Y");

	for (int i=0;i<=numOfRows-1;i++) {

		String aCell=db.at(i).at(0);

		if (std::regex_match (aCell,CURR)) {		
			if (i!=0) 
				bondRateMap.insert(std::make_pair(market,temp));
			temp.clear();
			market = EnumHelper::getCcyEnum(aCell);
			Market mkt(market);
			continue;
		}

		if (std::regex_match (aCell,YR)) {
			bondTenorNumOfMonths=std::stoi(aCell.substr(0,aCell.find("Y")))*12;
		} else {
			bondTenorNumOfMonths=std::stoi(aCell.substr(0,aCell.find("M")));
		}

		string maturityStr = db.at(i).at(1);
		date maturityDate(maturityStr);
		date tradeDate=dateUtil::getToday();
		int couponFreq=db.at(i).at(3)=="NaN"?NaN:std::stoi(db.at(i).at(3));
		double couponRate = std::stod(db.at(i).at(2))/(couponFreq==NaN?1:couponFreq)/100;
		double cleanPrice = std::stod(db.at(i).at(4));
		enum::DayCountEnum dayCount = EnumHelper::getDayCountEnum(db.at(i).at(5));
		double YTM = std::stod(db.at(i).at(6));

		Bond tempBond(market,tradeDate,maturityDate,bondTenorNumOfMonths,couponRate,couponFreq,Configuration::getInstance(),cleanPrice,YTM,dayCount);
		temp.insert(std::make_pair(maturityDate.getJudianDayNumber(),tempBond));	
	}
	bondRateMap.insert(std::make_pair(market,temp));
	RecordHelper::getInstance()->setBondRateMap(bondRateMap);

	_inFile.close();

}
