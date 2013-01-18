//created by Wang Jianwei on 1 Dec 2012
#include "TestDiscountCurve.h"
#include "SwapCurveBuilder.h"
#include "BondCurveBuilder.h"
#include "DiscountCurve.h"
#include <string>
#include <sstream>
#include "dateUtil.h"
#include "Enums.h"
#include "Configuration.h"
#include "dateUtil.h"
#include "swap.h"
#include "RecordHelper.h"
#include "Constants.h"
#include "marketdata.h"

using namespace UnitTest;
using namespace utilities;
using namespace std;
using namespace Markets;

void TestDiscountCurve::runTest(){
	_EPSILON = 0.000001;
	discountCurveTestSuit();
}

void TestDiscountCurve::discountCurveTestSuit(){
	swapRateTest(enums::USD, enums::LINEAR);
	swapRateTest(enums::USD, enums::LOGLINEAR);
	BondRateTest(enums::USD, enums::LINEAR);
	BondRateTest(enums::USD, enums::LOGLINEAR);
}

void TestDiscountCurve::swapRateTest(enums::MarketEnum market,enums::interpolAlgo interpolAlgo){
	cout<<"\n******** Swap Discount Curve Test using interpolation method ["<<interpolAlgo<<"] ********"<<endl;

	DiscountCurve* yc = MarketData::getInstance()->getSwapDiscountCurve();

	Market fixLegCurr=Market(market);
	Market floatingLegCurr=Market(market);
	int fixFreq = 2;
	int floatFreq = 4;

	map<long, double> swapRateMap = RecordHelper::getInstance()->getSwapRateMap()[market];
	for (map<long, double>::iterator it=swapRateMap.begin(); it != swapRateMap.end(); it++ ){
		date accuralEndDate = date((*it).first);
		Swap swap1(dateUtil::getToday(), accuralEndDate, 1000000, 0.01, yc, fixLegCurr, floatingLegCurr,fixFreq, floatFreq, true,1);
		cashflowLeg* fixLeg=swap1.getCashflowLegFix();
		cashflowLeg* floatLeg=swap1.getCashflowLegFloat();
		compareResult("Swap Discount Curve", accuralEndDate, swap1.getParRate(fixLeg,floatLeg,yc),(*it).second);
	}
}


void TestDiscountCurve::BondRateTest(enums::MarketEnum market, enums::interpolAlgo interpolAlgo){
	cout<<"\n******** Bond Discount Curve Test using interpolation method ["<<interpolAlgo<<"] ********"<<endl;

	DiscountCurve* dc = MarketData::getInstance()->getBondDiscountCurve();

	map<long, Bond> bondRateMap = RecordHelper::getInstance()->getBondRateMap()[market];
	for (map<long, Bond>::iterator it=bondRateMap.begin(); it != bondRateMap.end(); it++ ){
		date accuralEndDate = (*it).first;
		Bond bond = (*it).second;
		bond.setDiscountCurve(dc);
		if (bond.getCouponFreq()!=NaN){
			double expectedVal = bond.getDirtyPrice();
			double derivedVal = bond.getMPV();
			compareResult("Bond Discount Curve", accuralEndDate, derivedVal,expectedVal);
		}
	}
}

void TestDiscountCurve::compareResult(string testName, date accuralEndDate, double derivedVal,  double expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	ss.precision(10);
	bool result = AbstractTest::compareResult(derivedVal, expectedVal);	
	if (result)
		ss<<testName<<" Test Passed: Accural end date ["<<accuralEndDate.toString() << "], derived swap rate [" << derivedVal <<"], given swap rate ["<<expectedVal<<"], diff ["<<derivedVal-expectedVal<<"]"<<endl;
	else
		ss<<testName<<" Test Failed: Accural end date ["<<accuralEndDate.toString() << "], derived swap rate [" << derivedVal <<"], given swap rate ["<<expectedVal<<"], diff ["<<derivedVal-expectedVal<<"]"<< endl;

	cout<<ss.str();
}