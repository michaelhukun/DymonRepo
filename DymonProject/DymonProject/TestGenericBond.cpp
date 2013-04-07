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
		DiscountCurve* bondCurve =&(it->second);
		curveTest(currency, bondCurve);
	}
}

void TestGenericBond::curveTest(enums::CurrencyEnum currency, DiscountCurve* dc){

	for (unsigned int i =0; i<dc->getComponents()->size(); i++){
		double expectedVal = NaN;
		double derivedVal = NaN;
		AbstractInstrument* instrument = dc->getComponents()->at(i);
		if (instrument->getInstrumentEnum() == enums::BOND){
			Bond* bond = static_cast<Bond*>(instrument);
			BondPricer pricer = BondPricer(bond);
			pricer.setDiscountCurve(dc);
			expectedVal = bond->getDirtyPrice();
			derivedVal = pricer.getMPV();
		}
		compareResult(instrument->getID(),derivedVal, expectedVal);
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