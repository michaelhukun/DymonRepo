//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTBOND_H
#define TESTBOND_H
#include "AbstractTest.h"
#include "Enums.h"
#include "bond.h"
#include <string>

using namespace instruments;
using namespace std;

namespace UnitTest{
	class TestBond: public AbstractTest{

	public:
		TestBond():AbstractTest(){}

		void runTest();

		void yieldTestSuit();

		void gSpreadTestSuit();

	private:

		void yieldTest(Bond* bond, DiscountCurve* dc);

		void gSpreadTest(Bond* bond, DiscountCurve* dc);

		void compareResult(string testName, Bond* bond, double derivedVal, double expectedVal);
	};
}
#endif