#include "TestGenericSwap.h"
#include "marketdata.h"

using namespace UnitTest;
using namespace Markets;

void TestGenericSwap::runTest(){
	auto map = MarketData::getInstance()->getSwapCurveMap(); 
	for(auto it=map->begin(); it!=map->end(); it++){
		enums::CurrencyEnum currency = it->first;
		DiscountCurve* swapCurve =&(it->second);
		curveTest(currency, swapCurve);
	}
}

void TestGenericSwap::curveTest(enums::CurrencyEnum currency, DiscountCurve* dc){
	//auto map = 
	//for
}

void TestGenericSwap::compareResult(string testName, double derivedVal, double expectedVal){
}