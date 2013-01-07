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
	_fileName = cfg->getProperty("swaptionVolCube.usd.file",true,"");
	_persistDir = cfg->getProperty("swaptionVolCube.usd.path",false,"");
	AbstractFileSource::init(cfg);
}

void SwaptionVolFileSource::retrieveRecord(){
	AbstractFileSource::retrieveRecord();

	CSVDatabase db;
	readCSV(_inFile, db);

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
			enums::MarketEnum market = EnumHelper::getCcyEnum(db.at(i).at(1));
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

				double vol=db.at(i).at(j).compare("")==0?NaN:std::stod(db.at(i).at(j));
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

	cout <<"numofRows="<<db.size()<<endl;
	cout <<"numOfCols="<<db.at(0).size()<<endl;

	tempSwaptionCubeMap.insert(std::make_pair(strikeDiffATM,volSurfaceMap));
	RecordHelper::getInstance()->setSwaptionATMStrikeMap(tempSwaptionATMStrikeMap);
	RecordHelper::getInstance()->setSwaptionVolMap(tempSwaptionCubeMap);
	_inFile.close();
	//DAO::SwaptionVolFileSource::swaptionTest();
}

void SwaptionVolFileSource::readCSV(std::ifstream &input, CSVDatabase &db) {
	String csvLine;
	// read every line from the stream
	while( std::getline(input, csvLine) ){
		std::istringstream csvStream(csvLine);
		CSVRow csvRow;
		String csvCol;
		// read every element from the line that is seperated by commas
		// and put it into the vector or strings
		while( std::getline(csvStream, csvCol, ',') )
			csvRow.push_back(csvCol);
		db.push_back(csvRow);
	}

};

void SwaptionVolFileSource::display(const CSVRow& row) {
	if(!row.size())
		return;
	CSVRowCI i=row.begin();
	std::cout<<*(i++);
	for(;i != row.end();++i)
		std::cout<<','<<*i;
};

void SwaptionVolFileSource::display(const CSVDatabase& db) {
	if(!db.size())
		return;
	CSVDatabaseCI i=db.begin();
	for(; i != db.end(); ++i){
		display(*i);
		std::cout<<std::endl;
	}	
};


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
	display(db);

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


