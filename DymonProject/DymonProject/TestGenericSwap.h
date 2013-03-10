//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTGENERICSWAP_H
#define TESTGENERICSWAP_H
#include "AbstractTest.h"
#include "AbstractSurface.h"
#include "Enums.h"
#include "Market.h"
#include <string>

using namespace std;
using namespace utilities;

namespace UnitTest{
	class TestGenericSwap: public AbstractTest{

	public:
		TestGenericSwap():AbstractTest(){}

		void runTest();

	private:

		void surfaceTest(enums::CurrencyEnum currency);

		void compareResult(string testName, double derivedVal, double expectedVal);
	};
}
#endif