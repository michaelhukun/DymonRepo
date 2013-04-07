#include "TestGenericSwap.h"
#include "marketdata.h"
#include "RecordHelper.h"
#include "DepositPricer.h"
#include "SwapPricer.h"
#include "EuroDollarFuturePricer.h"
#include "AbstractInstrument.h"

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

	for (unsigned int i =0; i<dc->getComponents()->size(); i++){
		double expectedVal = NaN;
		double derivedVal = NaN;
		AbstractInstrument* instrument = dc->getComponents()->at(i);
		if (instrument->getInstrumentEnum() == enums::DEPOSIT || instrument->getInstrumentEnum() == enums::LIBOR){
			Deposit* deposit = static_cast<Deposit*>(instrument);
			DepositPricer pricer = DepositPricer(deposit);
			pricer.setDiscountCurve(dc);
			expectedVal = deposit->getRate();
			derivedVal = pricer.deriveDepositRate();
		}else if (instrument->getInstrumentEnum() == enums::EURODOLLARFUTURE){
			EuroDollarFuture* future = static_cast<EuroDollarFuture*>(instrument);
			EuroDollarFuturePricer pricer = EuroDollarFuturePricer(future);
			pricer.setDiscountCurve(dc);
			expectedVal = future->getRate();
			derivedVal = pricer.getMPV();
		}else if (instrument->getInstrumentEnum() == enums::SWAP){
			Swap* swap = static_cast<Swap*>(instrument);
			SwapPricer pricer = SwapPricer(swap);
			pricer.setDiscountCurve(dc);
			expectedVal = swap->getSwapRate();
			derivedVal = pricer.deriveSwapRate();
		}
		compareResult(instrument->getID(),derivedVal, expectedVal);
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