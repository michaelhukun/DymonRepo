#include "TestBond.h"
#include "marketdata.h"
#include "RecordHelper.h"
#include "Constants.h"

using namespace utilities;
using namespace UnitTest;
using namespace Markets;

void TestBond::runTest(){
	yieldTestSuit();
	gSpreadTestSuit();
}

void TestBond::yieldTestSuit(){
	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve();
	RecordHelper::BondRateMap* bondRateMap = RecordHelper::getInstance()->getBondRateMap();
	RecordHelper::BondRateMap::iterator bondMapIt;
	for (bondMapIt=bondRateMap->begin(); bondMapIt!=bondRateMap->end(); ++bondMapIt){
		map<long, Bond>* innerBondMap = &(bondMapIt->second);
		map<long, Bond>::iterator innerBondMapIt;
		for (innerBondMapIt=innerBondMap->begin(); innerBondMapIt!=innerBondMap->end(); ++innerBondMapIt){
			Bond* tempBond = &(innerBondMapIt->second);
			yieldTest(tempBond, dc);
		}
	}
}

void TestBond::gSpreadTestSuit(){
	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve();
	RecordHelper::BondRateMap* bondRateMap = RecordHelper::getInstance()->getBondRateMap();
	RecordHelper::BondRateMap::iterator bondMapIt;
	for (bondMapIt=bondRateMap->begin(); bondMapIt!=bondRateMap->end(); ++bondMapIt){
		map<long, Bond>* innerBondMap = &(bondMapIt->second);
		map<long, Bond>::iterator innerBondMapIt;
		for (innerBondMapIt=innerBondMap->begin(); innerBondMapIt!=innerBondMap->end(); ++innerBondMapIt){
			Bond* tempBond = &(innerBondMapIt->second);
			gSpreadTest(tempBond, dc);
		}
	}
}

void TestBond::yieldTest(Bond* bond, DiscountCurve* dc){
	double derivedYield = bond->getYield();
	double quotedYield = bond->getQuotedYTM();
	compareResult("Yield", bond, derivedYield, quotedYield);
}

void TestBond::gSpreadTest(Bond* bond, DiscountCurve* dc){
	double derivedGSpread = bond->getGspread(dc);
	double quotedGSpread = bond->getQuotedGSpread();
	if (quotedGSpread==NaN){
		cout<< "bond ID ["<<bond->getID()<<"], derived G-Spread ["<<derivedGSpread<<"], quoted G-Spread [N/A]"<<endl;
	}else{
		compareResult("G-Spread", bond, derivedGSpread, quotedGSpread);
	}
}

void TestBond::compareResult(string testName, Bond* bond, double derivedVal, double expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	bool result = AbstractTest::compareResult(derivedVal, expectedVal);	
	if (result)
		ss<<testName<<" Test Passed: bond ID ["<<bond->getID()<<"], val ["<<derivedVal<<"]";
	else
		ss<<testName<<" Test Failed:  bond ID ["<<bond->getID()<<"], derived val ["<<derivedVal<<"], quoted val ["<<expectedVal<<"]";

	cout<<ss.str()<<endl;
}