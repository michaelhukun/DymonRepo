//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTBUILDCASHFLOWLEG_H
#define TESTBUILDCASHFLOWLEG_H

#include "TestBuildCashFlowLeg.h"
#include <string>
#include "date.h"
#include "AbstractTest.h"
#include "cashflowLeg.h"
#include "Market.h"
#include "Enums.h"

using namespace std;
using namespace utilities;
using namespace instruments;

namespace UnitTest{
	class TestBuildCashFlowLeg: public AbstractTest{

	public:
		typedef tuple<date, double> point;

		TestBuildCashFlowLeg():AbstractTest(){}

		void runTest();

		void BuildCashFlowLegTestSuit();

	private:

		void buildCashFlowLegTest(date startDate, int numberOfMonth, double tolerance, enums::MarketEnum market);

		bool compareResult(cashflowLeg* cashflowFromMaturity, cashflowLeg* cashflowFromTenor);
	};
}
#endif