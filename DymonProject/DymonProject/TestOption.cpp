//created by Wang Jianwei on 1 Dec 2012
#include "TestOption.h"
#include "EuropeanOption.h"
#include "AbstractOption.h"
#include <string>
#include <sstream>
#include "dateUtil.h"
#include "Enums.h"

using namespace UnitTest;
using namespace utilities;
using namespace std;

void TestOption::runTest(){
	_EPSILON = 0.000001;
	europeanTestSuit();
	swaptionTestSuit();
}

void TestOption::europeanTestSuit(){
	europeanTest(Call,12,10,0.3,0.01,12,2.61195476);
}

void TestOption::swaptionTestSuit(){
}

void TestOption::europeanTest(enums::VolType VolTypeFlag, double S, double K, double vol, double r, int expiryInMonth, double expectedVal){
	Market market(enums::USD);
	AbstractOption* option = new EuropeanOption(market, dateUtil::getToday(),expiryInMonth,VolTypeFlag,S,K,vol,r);
	compareResult("European Option", option->getMPV(), expectedVal);
}

void TestOption::swaptionTest(enums::VolType VolTypeFlag, double S, double K, double vol, double r, int expiryInMonth, double expectedVal){

}

void TestOption::compareResult(string testName, double derivedVal, double expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	bool result = AbstractTest::compareResult(derivedVal, expectedVal);	
	if (result)
		ss<<testName<<" Test Passed: option price ["<<derivedVal<<"]";
	else
		ss<<testName<<" Test Failed: derived option price ["<<derivedVal<<"], expected option price ["<<expectedVal<<"]";

	cout<<ss.str()<<endl;
}