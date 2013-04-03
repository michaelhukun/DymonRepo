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
	class TestSwapCurve: public AbstractTest{

	public:
		TestSwapCurve():AbstractTest(){}

		void init(Configuration* cfg);

		void runTest();

		void discountCurveTestSuit();

		void constructExpectedCurveMap();

	private:

		void swapRateTest(enums::CurrencyEnum market, enums::interpolAlgo interpolAlgo);

		void BondRateTest(enums::CurrencyEnum market, enums::interpolAlgo interpolAlgo);

		void compareResult(string testName, date accuralEndDate, double derivedVal, double expectedVal);

		std::map<enums::CurrencyEnum, map<date, double>> _expectedCurveMap;

		enums::interpolAlgo _interpolAlgo;
	};
}
#endif