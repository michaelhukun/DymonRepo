//created by Hu Kun 1 Jan
#include "SwaptionVolFileSource.h"
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


void SwaptionVolFileSource::init(Configuration* cfg){
	Market market(EnumHelper::getCcyEnum("USD"));
   _name = "Swaption Vol";
	_fileName = cfg->getProperty("swaptionVolCube."+market.getNameString()+".file",true,"");
	_enabled = (cfg->getProperty("swaptionVolCube.enabled",true,"")=="true")?true:false;
	AbstractFileSource::init(cfg);
}

void SwaptionVolFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	AbstractFileSource::retrieveRecord();
	CSVDatabase db =	readCSV(_inFile, db);
	
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();
	int strikeDiffATM=0;
	
	RecordHelper::SwaptionCubeMap tempSwaptionCubeMap;
	RecordHelper::SwaptionATMStrikeMap tempSwaptionATMStrikeMap;
	RecordHelper::SwaptionSurfaceMap volSurfaceMap;
	std::regex ATM ("ATM(.*)");

	for (int i=0;i<=numOfRows-1;i++) {

		String aCell=db.at(i).at(0);

		if (std::regex_match (aCell,ATM)) {
			if (i!=0){				
				tempSwaptionCubeMap.insert(std::make_pair(strikeDiffATM,volSurfaceMap));
				//<double,std::map<tuple<double,double>,double>>
				volSurfaceMap.clear();
			}
			strikeDiffATM = getStrikeDiffATM(aCell);
			enums::CurrencyEnum market = EnumHelper::getCcyEnum(db.at(i).at(1));
			Market mkt(market);
			continue;
		}

		for (int j=2;j<=numOfCols-1;j++) {

			String topRowCell=db.at(0).at(j);
			String tagCell=db.at(i).at(1);

			if (tagCell.compare("Vol")==0) {
				int optionExpiryInMonth=std::stoi(aCell.substr(0,aCell.find(" ")));
				if (string::npos != aCell.find("YR")) {
					optionExpiryInMonth=std::stoi(aCell.substr(0,aCell.find(" ")))*12;
				}

				double vol=db.at(i).at(j).compare("")==0?NaN:std::stod(db.at(i).at(j))/100;
				double strike=db.at(i+1).at(j).compare("")==0?NaN:std::stod(db.at(i+1).at(j));
				int fSwapTenorInMonth=std::stoi(topRowCell.substr(0,topRowCell.find(" ")))*12;

				auto aTuple=std::make_tuple(fSwapTenorInMonth,optionExpiryInMonth);
				insertPointVolSurfaceMap(volSurfaceMap,fSwapTenorInMonth,optionExpiryInMonth,vol);
				if (strikeDiffATM==0)
					tempSwaptionATMStrikeMap.insert(std::make_pair(aTuple,strike));

			}
			else {
				continue;
			}
		}
	}

	tempSwaptionCubeMap.insert(std::make_pair(strikeDiffATM,volSurfaceMap));
	RecordHelper::getInstance()->setSwaptionATMStrikeMap(tempSwaptionATMStrikeMap);
	RecordHelper::getInstance()->setSwaptionVolMap(tempSwaptionCubeMap);
	_inFile.close();
}

int SwaptionVolFileSource::getStrikeDiffATM(string strikeStr){
	std::regex bps ("(.*)bps");
	if (std::regex_match (strikeStr,bps)) {
		string strikeStrATMRemoved = strikeStr.erase(0,3);
		int optionExpiryInMonth=std::stoi(strikeStrATMRemoved.substr(0,strikeStrATMRemoved.find("bps")));
		return optionExpiryInMonth;
	}
	return 0;
}

void SwaptionVolFileSource::swaptionTest() {

	std::fstream file("swaption_skew_USD.csv", std::ios::in);
	if(!file.is_open()){
		std::cout << "File not found!\n";
		return;
	}
	CSVDatabase db;
	readCSV(_inFile, db);
};

void SwaptionVolFileSource::insertPointVolSurfaceMap(RecordHelper::SwaptionSurfaceMap &map, int fSwapTenorInMonth, int optionExpiryInMonth, double vol){
	if (map.find(fSwapTenorInMonth)==map.end()){
		std::map<int,double> volCurveMap;
		volCurveMap.insert(std::make_pair(optionExpiryInMonth, vol));
		map.insert(std::make_pair(fSwapTenorInMonth, volCurveMap));
	}else{
		std::map<int,double>* volCurveMap = &map[fSwapTenorInMonth];
		volCurveMap->insert(std::make_pair(optionExpiryInMonth, vol));
	}
}


