#include "FXSkewFileSource.h"
#include <regex>
#include "Constants.h"
#include "EnumHelper.h"
#include "DeltaVol.h"
#include "Constants.h"

using namespace DAO;
using namespace utilities;

void FXSkewFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("FXSkew.file",true,"");
	_persistDir = cfg->getProperty("FXSkew.path",false,"");
	_enabled = cfg->getProperty("FXSkew.enabled",true,"")=="true"?true:false;
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

	for (int i=1;i<numOfRows;i++) {
		string securityID = db.at(i).at(0);
		double vol = std::stod(db.at(i).at(1));
		parseRow(securityID, vol);
	}

	RecordHelper::FXVolSkewMap* FXVolSkewMap = RecordHelper::getInstance()->getFXVolSkewMap();

	_inFile.close();
}

void FXSkewFileSource::parseRow(std::string securityID, double vol){
	string ccyPairStr = securityID.substr(0,6);
	unsigned found = securityID.find(" BGN Curncy");
	if (found==std::string::npos)
		throw "Security ID not recognized.";
	string deltaTenorStr = securityID.erase(found).substr(6);
	int tenorIndex = getTenorIndex(deltaTenorStr);
	double delta = getDelta(deltaTenorStr.substr(0,tenorIndex-1));
	enums::OptionType optionType = EnumHelper::getOptionType(deltaTenorStr.substr(tenorIndex-1,1));
	string tenorStr = deltaTenorStr.substr(tenorIndex, deltaTenorStr.length()-tenorIndex-1);
	char tenorUnit = *deltaTenorStr.rbegin();
	double tenorExpiry = getTenorExpiry(tenorStr, tenorUnit, USD);
	double tenorDiscount = getTenorDiscount(tenorStr, tenorUnit, USD);
	insertFXVolIntoCache(ccyPairStr, tenorExpiry, tenorDiscount, delta,optionType, vol);
}

double FXSkewFileSource::getDelta(std::string deltaStr){
	if (deltaStr.length()==0)
		return NaN;
	else return stod(deltaStr);
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


double FXSkewFileSource::getTenorExpiry(string tenorStr, char tenorUnit, enums::CurrencyEnum marketEnum){
	int tenor = (tenorUnit=='N')?1:stoi(tenorStr);
	if (tenorUnit!='Y'){
		Market market(marketEnum);
		date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following, marketEnum);
		date expiryDate  = dateUtil::getEndDate(startDate,tenor, market.getDayRollCashConvention(), market.getCurrencyEnum(), dateUtil::getDateUnit(tenorUnit));
		int daysToExpiry = dateUtil::getDaysBetween(startDate, expiryDate);
		return daysToExpiry / numDaysInYear;
	}
	return tenor;
}

double FXSkewFileSource::getTenorDiscount(string tenorStr, char tenorUnit, enums::CurrencyEnum marketEnum){
	int tenor = (tenorUnit=='N')?1:stoi(tenorStr);
	Market market(marketEnum);
	date startDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following, marketEnum);
	date endDate  = dateUtil::getEndDate(startDate,tenor, market.getDayRollCashConvention(), market.getCurrencyEnum(), dateUtil::getDateUnit(tenorUnit));
	date deliveryDate = dateUtil::getBizDateOffSet(endDate,market.getBusinessDaysAfterSpot(enums::SWAP), market.getCurrencyEnum());
	int daysToDiscount = dateUtil::getDaysBetween(startDate, deliveryDate);
	double tenorDiscount = daysToDiscount / numDaysInYear;
	return tenorDiscount;
}

void FXSkewFileSource::insertFXVolIntoCache(std::string ccyPairStr, double tenorExpiry, double tenorDiscount, double delta, OptionType optionType, double vol){
	
	RecordHelper::FXVolSkewMap* FXVolSkewMap = RecordHelper::getInstance()->getFXVolSkewMap();

	if (FXVolSkewMap->find(ccyPairStr) == FXVolSkewMap->end()){
		map<double, vector<DeltaVol>> tempMap = map<double, vector<DeltaVol>>();
		FXVolSkewMap->insert(std::make_pair(ccyPairStr, tempMap));
	}
	
	map<double, vector<DeltaVol>>* tempMap = &(FXVolSkewMap->find(ccyPairStr)->second);

	if (tempMap->find(tenorExpiry) == tempMap->end()){
		vector<DeltaVol> tempVector = vector<DeltaVol>();
		tempMap->insert(std::make_pair(tenorExpiry, tempVector));
	}
	
	vector<DeltaVol>* tempVector = &(tempMap->find(tenorExpiry)->second);
	DeltaVol deltaVol = DeltaVol(optionType, delta, tenorExpiry, tenorDiscount, vol);

	tempVector->push_back(deltaVol);
}
