//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTOPTION_H
#define TESTOPTION_H
#include "AbstractTest.h"
#include "Enums.h"
#include <string>

using namespace std;

namespace UnitTest{
	class TestOption: public AbstractTest{

	public:
		TestOption():AbstractTest(){}

		void runTest();

		void europeanTestSuit();

		void swaptionTestSuit();

	private:

		void europeanTest(enums::VolType VolTypeFlag, double S, double K, double vol, double r, int expiryInMonth, double expectedVal);

		void swaptionTest(enums::VolType VolTypeFlag, double S, double K, double vol, double r, int expiryInMonth, double expectedVal);

		void compareResult(string testName, double derivedVal, double expectedVal);
	};
}
#endif