//created by Wang Jianwei on 1 Dec 2012
#include "TestDiscountCurve.h"
#include "DiscountCurveBuilder.h"
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

using namespace UnitTest;
using namespace utilities;
using namespace std;

void TestDiscountCurve::runTest(){
	_EPSILON = 0.000001;
	discountCurveTestSuit();
}

void TestDiscountCurve::discountCurveTestSuit(){
	swapRateTest(enums::USD, enums::LINEAR);
	swapRateTest(enums::USD, enums::LOGLINEAR);
}

void TestDiscountCurve::swapRateTest(enums::MarketEnum market,enums::interpolAlgo interpolAlgo){
	cout<<"\n******** Discount Curve Test using interpolation method ["<<interpolAlgo<<"] ********"<<endl;

	DiscountCurveBuilder* builder = new DiscountCurveBuilder();
	//builder->init(Configuration::getInstance());
	builder->setMarket(Market(market));
	builder->setBizDaysAfterSpot(2);
	builder->setFloatFrequency(4);
	builder->setFixFreqency(2);
	builder->setNumericalAlgo(enums::BISECTION);
	builder->setInterpolAlgo(interpolAlgo);
	builder->setTimeLineBuildDirection(1);
	builder->setBizDaysAfterSpotDF(NaN);
	DiscountCurve* yc = builder->build();
	cout<<yc->toString()<<endl;
	

	Market fixLegCurr=Market(market);
	Market floatingLegCurr=Market(market);

	map<long, double> swapRateMap = RecordHelper::getInstance()->getSwapRateMap()[market];
	for (map<long, double>::iterator it=swapRateMap.begin() ; it != swapRateMap.end(); it++ ){
		date accuralEndDate = date((*it).first);
		Swap swap1(dateUtil::getToday(), accuralEndDate, 1000000, 0.01, yc, fixLegCurr, floatingLegCurr,builder->getFixFreqency(), builder->getFloatFrequency(), true,1);
		cashflowLeg* fixLeg=swap1.getCashflowLegFix();
		cashflowLeg* floatLeg=swap1.getCashflowLegFloat();
		compareResult("Discount Curve", accuralEndDate, swap1.getParRate(fixLeg,floatLeg,yc),(*it).second);
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