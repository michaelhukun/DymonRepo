//created by Wang Jianwei on 1 Dec 2012
#include "TestSwapCurve.h"
#include "SwapCurveBuilder.h"
#include "DiscountCurve.h"
#include <string>
#include <sstream>
#include "dateUtil.h"
#include "Enums.h"
#include "Configuration.h"
#include "dateUtil.h"
#include "RecordHelper.h"
#include "Constants.h"
#include "marketdata.h"
#include "Market.h"

using namespace UnitTest;
using namespace utilities;
using namespace std;
using namespace Markets;

void TestSwapCurve::init(Configuration* cfg){
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("SwapDiscountCurve.interpol",false,"LINEAR"));
}

void TestSwapCurve::runTest(){
	_EPSILON = 0.000001;
	discountCurveTestSuit();
}

void TestSwapCurve::discountCurveTestSuit(){
	swapRateTest(enums::USD, _interpolAlgo);
	swapRateTest(enums::SGD, _interpolAlgo);
}

void TestSwapCurve::swapRateTest(enums::CurrencyEnum market,enums::interpolAlgo interpolAlgo){
	cout<<"\n******** Swap Discount Curve Test using interpolation method ["<<interpolAlgo<<"] ********"<<endl;

	DiscountCurve* yc = MarketData::getInstance()->getSwapDiscountCurve(market);
	map<date, double> expectedCurve = _expectedCurveMap.at(

}


void TestSwapCurve::BondRateTest(enums::CurrencyEnum market, enums::interpolAlgo interpolAlgo){
	cout<<"\n******** Bond Discount Curve Test using interpolation method ["<<interpolAlgo<<"] ********"<<endl;

	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve(USD);

	auto bondRateMap = RecordHelper::getInstance()->getBondRateMap()->at(market);
	for (auto it=bondRateMap.begin(); it != bondRateMap.end(); it++ ){
		date accuralEndDate = (*it).first;
		Bond bond = (*it).second;
		if (bond.getCouponFreq()!=NaN){
			double expectedVal = bond.getDirtyPrice();
			double derivedVal = bond.getMPV(dc);
			compareResult("Bond Discount Curve", accuralEndDate, derivedVal,expectedVal);
		}
	}
}

void TestSwapCurve::constructExpectedCurveMap(){
	map<date, double> curveUSD;
	curveUSD.insert(std::make_pair(date(2010,2,1),0.99999));
	_expectedCurveMap.insert(std::make_pair(USD, curveUSD));
	map<date, double> curveSGD;
	curveSGD.insert(std::make_pair(date(2010,2,1),0.99999));
	_expectedCurveMap.insert(std::make_pair(SGD, curveSGD));
}

void TestSwapCurve::compareResult(string testName, date accuralEndDate, double derivedVal,  double expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	ss.precision(10);
	bool result = AbstractTest::compareResult(derivedVal, expectedVal);	
	if (result)
		ss<<testName<<" Test Passed: Accural end date ["<<accuralEndDate.toString() << "], derived swap rate [" << derivedVal <<"], given swap rate ["<<expectedVal<<"], diff ["<<derivedVal-expectedVal<<"]"<<endl;
	else
		ss<<testName<<" Test Failed: Accural end date ["<<accuralEndDate.toString() << "], derived swap rate [" << derivedVal <<"], given swap rate ["<<expectedVal<<"], diff ["<<derivedVal-expectedVal<<"]"<< endl;

	cout<<ss.str();
}