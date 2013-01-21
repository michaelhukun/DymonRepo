//created by Hu Kun 13 Dec
#ifndef CASHFLOWLEG_H
#define CASHFLOWLEG_H

#include "cashflow.h"
#include "Market.h"
#include "Enums.h"
#include "date.h"
#include "dateUtil.h"
#include <vector>
#include <iterator>

using namespace utilities;
using namespace std;
using namespace enums;

namespace instruments {
	class cashflowLeg {

	public:
		cashflowLeg(){};
		cashflowLeg(vector<cashflow> cashflowLeg);
		~cashflowLeg(){};
		
		cashflow getCashFlow(unsigned int index);
		int getFirstValidCashFlowIndex();
		vector<cashflow> getCashFlowVector();
		void setCashFlowVector(vector<cashflow> cashflowLeg);
		vector<date> getAccuralStartDates();
		vector<date> getAccuralEndDates();
		void markCashFlowValidity(date tradeDate);
		int getCashFlowIndexForAccrualEnd(date accuralEndDate);

		void printCashFlowLeg();
		void printTimeLine();

	private:
		vector<cashflow> _cashflowLeg;

	};
}
#endif