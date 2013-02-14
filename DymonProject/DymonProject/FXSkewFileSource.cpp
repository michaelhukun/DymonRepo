#include "FXSkewFileSource.h"
#include <regex>
#include "Constants.h"
#include "EnumHelper.h"

using namespace DAO;
using namespace utilities;

std::regex overnightRegex ("[A-Z]{6}0N BGN Curncy");
std::regex weekRegex ("[A-Z]{6}V[0-9]*W BGN Curncy");
std::regex monthRegex ("[A-Z]{6}V[0-9]*M BGN Curncy");
std::regex yearRegex ("[A-Z]{6}V[0-9]*Y BGN Curncy");


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

	RecordHelper::FXVolSkewMap* FXSkewMap = RecordHelper::getInstance()->getFXVolSkewMap();

	for (int i=1;i<numOfRows;i++) {
		string securityID = db.at(i).at(0);
		double vol = std::stod(db.at(i).at(1));
		parseRow(securityID, vol, FXSkewMap);
	}

	_inFile.close();
}

void FXSkewFileSource::parseRow(std::string securityID, double vol, RecordHelper::FXVolSkewMap* FXVolSkewMap){
	string ccyPairStr = securityID.substr(0,6);
	unsigned found = securityID.find(" BGN Curncy");
	if (found==std::string::npos)
		throw "Security ID not recognized.";
	string deltaTenorStr = securityID.erase(found).substr(6);
	int tenorIndex = getTenorIndex(deltaTenorStr);
	int delta = getDelta(deltaTenorStr.substr(0,tenorIndex-1));
	enums::OptionType optionType = EnumHelper::getOptionType(deltaTenorStr.substr(tenorIndex-1,1));
	int daysToExpiry = getDaysToExpiry(deltaTenorStr, tenorIndex, USD);
	insertFXVolIntoCache(ccyPairStr, daysToExpiry, delta,optionType, vol, FXVolSkewMap);
}

int FXSkewFileSource::getDelta(std::string deltaStr){
	if (deltaStr.length()==0)
		return 0;
	else return stoi(deltaStr);
}

int FXSkewFileSource::getTenorIndex(std::string deltaTenorStr){
	int tenorIndex = 0;
	if (deltaTenorStr.find("V")!=string::npos){
		tenorIndex = deltaTenorStr.find("V");
	}else if (deltaTenorStr.find("R")!=string::npos){
		tenorIndex = deltaTenorStr.find("R");
	}else if(deltaTenorStr.find("B")!=string::npos){
		tenorIndex = deltaTenorStr.find("B");
	}else{
		throw "Security ID not recognized.";
	}
	return tenorIndex+1;
}


int FXSkewFileSource::getDaysToExpiry(std::string deltaTenorStr, int tenorIndex, enums::CurrencyEnum marketEnum){
	string tenorStr = deltaTenorStr.substr(tenorIndex, deltaTenorStr.length()-tenorIndex-1);
	int tenor = (deltaTenorStr.find("N")!=string::npos)?1:stoi(tenorStr);
	char tenorUnit = *deltaTenorStr.rbegin();
	Market market(marketEnum);
	date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following, marketEnum);
	date endDate  = dateUtil::getEndDate(startDate,tenor, market.getDayRollCashConvention(), market.getCurrencyEnum(), dateUtil::getDateUnit(tenorUnit));
	int daysToExpiry = dateUtil::getDaysBetween(startDate, endDate);
	return daysToExpiry;
}

void FXSkewFileSource::insertFXVolIntoCache(std::string currencyPairStr, int daysToExpiry, int delta, OptionType optionType, double vol, RecordHelper::FXVolSkewMap* FXVolSkewMap){

	if (FXVolSkewMap->find(currencyPairStr) == FXVolSkewMap->end()){
		map<int, vector<tuple<OptionType, int, double>>> tempMap = map<int, vector<tuple<OptionType, int, double>>>();
		FXVolSkewMap->insert(std::make_pair(currencyPairStr, tempMap));
	}
	
	map<int, vector<tuple<OptionType, int, double>>>* tempMap = &(FXVolSkewMap->find(currencyPairStr)->second);

	if (tempMap->find(daysToExpiry) == tempMap->end()){
		vector<tuple<OptionType, int, double>> tempVector = vector<tuple<OptionType, int, double>>();
		tempMap->insert(std::make_pair(daysToExpiry, tempVector));
	}
	
	vector<tuple<OptionType, int, double>>* tempVector = &(tempMap->find(daysToExpiry)->second);
	tuple<OptionType, int, double> tempTuple = std::make_tuple (optionType, delta, vol);

	tempVector->push_back(tempTuple);
}
