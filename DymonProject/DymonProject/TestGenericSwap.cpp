#include "TestGenericSwap.h"
#include "marketdata.h"
#include "RecordHelper.h"
#include "DepositPricer.h"
#include "SwapPricer.h"
#include "EuroDollarFuturePricer.h"

using namespace UnitTest;
using namespace Markets;
using namespace utilities;

void TestGenericSwap::init(Configuration* cfg){
	_isEnabled = cfg->getProperty("UnitTest.GenericSwap",true,"")=="true"?true:false;
	_EPSILON = 0.0000000001;
}

void TestGenericSwap::runTest(){
	if (!_isEnabled) return;
	auto map = MarketData::getInstance()->getSwapCurveMap(); 
	for(auto it=map->begin(); it!=map->end(); it++){
		enums::CurrencyEnum currency = it->first;
		DiscountCurve* swapCurve =&(it->second);
		curveTest(currency, swapCurve);
	}
}

void TestGenericSwap::curveTest(enums::CurrencyEnum currency, DiscountCurve* dc){
	auto map = RecordHelper::getInstance()->getDepositRateMap()->at(currency);
	for (auto it=map.begin(); it!=map.end(); it++){
		Deposit* deposit = &(it->second);
		if (!dc->existComponent(deposit)) continue;
		DepositPricer pricer = DepositPricer(deposit);
		pricer.setDiscountCurve(dc);
		double expectedVal = deposit->getRate();
		double derivedVal = pricer.deriveDepositRate();
		compareResult(deposit->getID(),derivedVal, expectedVal);
	}

	auto futureMap = RecordHelper::getInstance()->getEuroDollarFutureMap()->at(currency);
	for (auto it=futureMap.begin(); it!=futureMap.end(); it++){
		EuroDollarFuture* future = &(it->second);
		if (!dc->existComponent(future)) continue;
		EuroDollarFuturePricer pricer = EuroDollarFuturePricer(future);
		pricer.setDiscountCurve(dc);
		double expectedVal = future->getRate();
		double derivedVal = pricer.getMPV();
		compareResult(future->getID(),derivedVal, expectedVal);
	}

	auto swapMap = RecordHelper::getInstance()->getSwapRateMap()->at(currency);
	for (auto it=swapMap.begin(); it!=swapMap.end(); it++){
		Swap* swap = &(it->second);
		if (!dc->existComponent(swap)) continue;
		SwapPricer pricer = SwapPricer(swap);
		pricer.setDiscountCurve(dc);
		double expectedVal = swap->getSwapRate();
		double derivedVal = pricer.deriveSwapRate();
		compareResult(swap->getID(),derivedVal, expectedVal);
	}
}

void TestGenericSwap::compareResult(string testName, double derivedVal, double expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	bool result = AbstractTest::compareResult(derivedVal, expectedVal);	
	if (result)
		ss<<testName<<" Test Passed: rate ["<<derivedVal<<"]";
	else
		ss<<testName<<" Test Failed: derived rate ["<<derivedVal<<"], expected rate ["<<expectedVal<<"]";

	cout<<ss.str()<<endl;
}