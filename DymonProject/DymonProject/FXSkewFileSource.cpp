#include "FXSkewFileSource.h"

using namespace DAO;

void FXSkewFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("FXSkew.file",true,"");
	_persistDir = cfg->getProperty("FXSkew.path",false,"");
	_enabled = cfg->getProperty("FXSkew.enabled",true,"")=="True"?true:false;
	AbstractFileSource::init(cfg);
}

void FXSkewFileSource::retrieveRecord(){
	if (!_enabled) return;

	AbstractFileSource::retrieveRecord();
	CSVDatabase db;
	readCSV(_inFile, db);

	int numOfRows=db.size();
	int numOfCols=db.at(0).size();
	int bondTenorNumOfMonths=0;

	RecordHelper::FXVolSkewMap* bondRateMap = RecordHelper::getInstance()->getFXVolSkewMap();

	for (int i=1;i<numOfRows;i++) {
		string securityID = db.at(i).at(0);
		double vol = std::stod(db.at(i).at(1));
		parseRow(securityID, vol);
	}

	_inFile.close();
}

void FXSkewFileSource::parseRow(std::string securityID, double vol){
	

}

void FXSkewFileSource::insertFXVolIntoCache(CurrencyEnum CurrencyEnum, int expiry, int delta, double vol, RecordHelper::FXVolSkewMap* FXVolSkewMap){
	
	//if (FXVolSkewMap->find(CurrencyEnum) == FXVolSkewMap->end()){
	//	map<std::tuple<int,int>,double> tempMap = map<std::tuple<int,int>,double>();
	//	tempMap.insert(std::make_pair(maturityDateJDN, vol));
	//	FXVolSkewMap->insert(std::make_pair(CurrencyEnum, tempMap));
	//}else{
	//	map<std::tuple<int,int>,double>* tempMap = &(FXVolSkewMap->find(CurrencyEnum)->second);
	//	tempMap->insert(std::make_pair(maturityDateJDN, vol));
	//}
	//cout<<bond->toString()<<endl;
}
