#include "TestBond.h"
#include "marketdata.h"
#include "RecordHelper.h"
#include "Constants.h"

using namespace utilities;
using namespace UnitTest;
using namespace Markets;

void TestBond::runTest(){
	yieldTestSuit();
	yieldSpreadTestSuit();
	zeroRateSpreadTestSuit();
}

void TestBond::yieldTestSuit(){
	cout<<endl<<"*********** Bond Yield Test ************"<<endl<<endl;
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

void TestBond::yieldSpreadTestSuit(){
	cout<<endl<<"*********** Bond Yield Spread Test ************"<<endl<<endl;
	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve();
	RecordHelper::BondRateMap* bondRateMap = RecordHelper::getInstance()->getBondRateMap();
	RecordHelper::BondRateMap::iterator bondMapIt;
	for (bondMapIt=bondRateMap->begin(); bondMapIt!=bondRateMap->end(); ++bondMapIt){
		map<long, Bond>* innerBondMap = &(bondMapIt->second);
		map<long, Bond>::iterator innerBondMapIt;
		for (innerBondMapIt=innerBondMap->begin(); innerBondMapIt!=innerBondMap->end(); ++innerBondMapIt){
			Bond* tempBond = &(innerBondMapIt->second);
			yieldSpreadTest(tempBond, dc);
		}
	}
}


void TestBond::zeroRateSpreadTestSuit(){
	cout<<endl<<"*********** Bond Zero Rate Spread Test ************"<<endl<<endl;
	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve();
	RecordHelper::BondRateMap* bondRateMap = RecordHelper::getInstance()->getBondRateMap();
	RecordHelper::BondRateMap::iterator bondMapIt;
	for (bondMapIt=bondRateMap->begin(); bondMapIt!=bondRateMap->end(); ++bondMapIt){
		map<long, Bond>* innerBondMap = &(bondMapIt->second);
		map<long, Bond>::iterator innerBondMapIt;
		for (innerBondMapIt=innerBondMap->begin(); innerBondMapIt!=innerBondMap->end(); ++innerBondMapIt){
			Bond* tempBond = &(innerBondMapIt->second);
			zeroRateSpreadTest(tempBond, dc);
		}
	}
}

void TestBond::yieldTest(Bond* bond, DiscountCurve* dc){
	double derivedYield = bond->getYield();
	double quotedYield = bond->getQuotedYTM();
	compareResult("Yield", bond, derivedYield, quotedYield);
}

void TestBond::yieldSpreadTest(Bond* bond, DiscountCurve* dc){
	double derivedYieldSpread = bond->getYieldSpread(dc);
	cout<< "bond ID ["<<bond->getID()<<"], isGeneric ["<<bond->getIsGeneric()<<"], yield spread ["<<derivedYieldSpread<<"]"<<endl;
}

void TestBond::zeroRateSpreadTest(Bond* bond, DiscountCurve* dc){
	double zeroRateSpread = bond->getZeroRateSpread(dc);
	cout<< "bond ID ["<<bond->getID()<<"], isGeneric ["<<bond->getIsGeneric()<<"], zero rate spread ["<<zeroRateSpread<<"]"<<endl;
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