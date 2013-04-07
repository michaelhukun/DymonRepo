//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTGENERICBOND_H
#define TESTGENERICBOND_H
#include "AbstractTest.h"
#include "AbstractSurface.h"
#include "Enums.h"
#include "Market.h"
#include "DiscountCurve.h"
#include <string>

using namespace std;
using namespace utilities;

namespace UnitTest{
	class TestGenericBond: public AbstractTest{

	public:
		TestGenericBond():AbstractTest(){}

		void init(Configuration* cfg);

		void runTest();

	private:

		void curveTest(enums::CurrencyEnum currency, DiscountCurve* dc);

		void compareResult(string testName, double derivedVal, double expectedVal);
	};
}
#endif