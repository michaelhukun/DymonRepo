//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTDATEUTIL_H
#define TESTDATEUTIL_H

#include <string>
#include "date.h"
#include "AbstractTest.h"
#include "dateUtil.h"

using namespace std;
using namespace utilities;

namespace UnitTest{
	class TestDateUtil: public AbstractTest{

	public:
		typedef tuple<date, double> point;

		TestDateUtil():AbstractTest(){}

		void init(Configuration* cfg);

		void runTest();

		void dayRollAdjustTestSuit();

		void getEndDateTestSuit();

		void DayCountTestSuit();

		void bizDateAdjustSuit();

	private:

		void compareResult(std::string testName, date derivedVal, date expectedVal);
	};
}
#endif