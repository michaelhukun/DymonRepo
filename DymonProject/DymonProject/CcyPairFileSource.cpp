#include "CcyPairFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "RecordHelper.h"
#include "Enums.h"
#include "EnumHelper.h"
#include "Market.h"
#include "CcyPair.h"

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;
using namespace instruments;

void CcyPairFileSource::init(Configuration* cfg){
	_name = "Currency Pair";
	_fileName = cfg->getProperty("CcyPair.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("CcyPair.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void CcyPairFileSource::retrieveRecord(){
	if (!_enabled) return;
	
	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	RecordHelper::CcyPairMap* CcyPairMap = RecordHelper::getInstance()->getCcyPairMap();

	for (int i=1;i<numOfRows;i++) {
		CcyPair* tempCcyPair = createCcyPairObject(db, i);
		insertCcyPairIntoCache(tempCcyPair, CcyPairMap);
	}
}

// NOT SURE ABOUT THIS ONE
void CcyPairFileSource::insertCcyPairIntoCache(CcyPair* ccypair, RecordHelper::CcyPairMap* CcyPairMap){
	// Do currencies have deliverydates? no?
	// 
	if (CcyPairMap->find(ccyPairStr) == CcyPairMap->end()){
		auto tempMap = map<long, CcyPair>();
		tempMap.insert(std::make_pair(deliveryDateJDN, *CcyPair));
		CcyPairMap->insert(std::make_pair(ccyPairStr, tempMap));
	}else{
		auto tempMap = &(CcyPairMap->find(ccyPairStr)->second);
		tempMap->insert(std::make_pair(deliveryDateJDN, *CcyPair));
	}
	//cout<<forward->toString()<<endl;
}

CcyPair* CcyPairFileSource::createCcyPairObject(CSVDatabase db, int row){
	int numOfCols=db.at(0).size();
	CcyPair* tempCcyPair = new CcyPair();

	for (int i=0;i<numOfCols;i++) {
		String fieldName = db.at(0).at(i);
		String fieldVal = db.at(row).at(i);
		updateCcyPairObjectField(fieldName, fieldVal, tempCcyPair);
	}		
	return tempCcyPair;
}

void CcyPairFileSource::updateCcyPairObjectField(std::string fieldName, std::string fieldVal, CcyPair* ccypair){
	if (fieldName=="CCYPAIR"){
		ccypair->setCcyPairStr(fieldVal);
	}else if (fieldName=="PREMIUM_INCLUDED_DELTA"){
		ccypair->;
	}else if (fieldName=="FORWARD_DELTA"){
		ccypair->;
	}
}



		



		


