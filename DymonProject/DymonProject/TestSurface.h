//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTSURFACE_H
#define TESTSURFACE_H
#include "AbstractTest.h"
#include "AbstractSurface.h"
#include "Enums.h"
#include "Market.h"
#include <string>

using namespace std;
using namespace utilities;

namespace UnitTest{
	class TestSurface: public AbstractTest{

	public:
		TestSurface():AbstractTest(){}

		void runTest();

		void surfaceTestSuit();

	private:

		void surfaceTest();

		void compareResult(string testName, double derivedVal, double expectedVal);
	};
}
#endif