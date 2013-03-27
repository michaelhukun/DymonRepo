#include "TestGenericBond.h"
#include "marketdata.h"
#include "RecordHelper.h"
#include "BondPricer.h"

using namespace UnitTest;
using namespace Markets;
using namespace utilities;

void TestGenericBond::init(Configuration* cfg){
	_isEnabled = cfg->getProperty("UnitTest.GenericBond",true,"")=="true"?true:false;
	_EPSILON = 0.0000000001;
}

void TestGenericBond::runTest(){
	if (!_isEnabled) return;
	auto map = MarketData::getInstance()->getBondCurveMap(); 
	for(auto it=map->begin(); it!=map->end(); it++){
		enums::CurrencyEnum currency = it->first;
		DiscountCurve* swapCurve =&(it->second);
		curveTest(currency, swapCurve);
	}
}

void TestGenericBond::curveTest(enums::CurrencyEnum currency, DiscountCurve* dc){
	auto map = RecordHelper::getInstance()->getBondRateMap()->at(currency);
	for (auto it=map.begin(); it!=map.end(); it++){
		Bond* bond = &(it->second);
		if (bond->getIsBill() || !dc->existComponent(bond)) continue;
		BondPricer pricer = BondPricer(bond);
		pricer.setDiscountCurve(dc);
		double expectedVal = bond->getDirtyPrice();
		double derivedVal = pricer.getMPV();
		compareResult(bond->getID(),derivedVal, expectedVal);
	}

}

void TestGenericBond::compareResult(string testName, double derivedVal, double expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	bool result = AbstractTest::compareResult(derivedVal, expectedVal);	
	if (result)
		ss<<testName<<" Test Passed: rate ["<<derivedVal<<"]";
	else
		ss<<testName<<" Test Failed: derived rate ["<<derivedVal<<"], expected rate ["<<expectedVal<<"]";

	cout<<ss.str()<<endl;
}