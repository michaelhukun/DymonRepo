//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTDISCOUNTCURVE_H
#define TESTDISCOUNTCURVE_H
#include "AbstractTest.h"
#include "DiscountCurve.h"
#include "Enums.h"
#include "Market.h"
#include <string>

using namespace std;
using namespace utilities;

namespace UnitTest{
	class TestDiscountCurve: public AbstractTest{

	public:
		TestDiscountCurve():AbstractTest(){}

		void runTest();

		void discountCurveTestSuit();

	private:

		void swapRateTest(enums::MarketEnum market, enums::interpolAlgo interpolAlgo);

		void BondRateTest(enums::MarketEnum market, enums::interpolAlgo interpolAlgo);

		void compareResult(string testName, date accuralEndDate, double derivedVal, double expectedVal);
	};
}
#endif