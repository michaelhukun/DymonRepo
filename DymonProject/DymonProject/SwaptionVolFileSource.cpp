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

	string value;
	enums::MarketEnum market;
	market = EnumHelper::getCcyEnum(db.at(0).at(0));
	Market mkt(market);
	//std::map<strike,std::map<tuple<double fSwapTenorNumOfMonths,double optionTenorNumOfMonths>,double swaptionVol>> SwaptionVolMap

	RecordHelper::SwaptionVolMap tempMap;
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	for (int i=1;i<=numOfRows-1;i++) {
		for (int j=1;j<=numOfCols-1;j++) {

			auto aTuple=std::make_tuple(std::stod(db.at(0).at(j)),std::stod(db.at(i).at(0)));
			tempMap.insert(pair<tuple<double,double>,double>(aTuple,std::stod(db.at(i).at(j))));

		}

	}

	RecordHelper::getInstance()->setSwaptionVolMap(tempMap);
	_inFile.close();
}

void SwaptionVolFileSource::readCSV(std::ifstream &input, CSVDatabase &db) {
	String csvLine;
	// read every line from the stream
	while( std::getline(input, csvLine) ){
		std::istringstream csvStream(csvLine);
		CSVRow csvRow;
		string csvCol;
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

void SwaptionVolFileSource::swaptionTest() {

	std::fstream file("SwaptionATMVolMatrix_USD.csv", std::ios::in);
	if(!file.is_open()){
		std::cout << "File not found!\n";
		return;
	}
	CSVDatabase db;
	readCSV(_inFile, db);
	display(db);

};
