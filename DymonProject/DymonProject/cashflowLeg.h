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
		cashflowLeg(vector<cashflow> cashflowLeg, int couponFreq);
		~cashflowLeg(){};
		
		// Getters and Setters
		int getCouponFreq(){return _couponFreq;}
		int getSize(){return _cashflowVector.size();}
		enums::DayCountEnum getDayCount(){ return _dayCount; }
		int getCashFlowNumber(){ return _cashFlowNumber; }

		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}
		void setCouponFreq(int couponFreq){ _couponFreq = couponFreq; }
		void setCashFlowNumber(int cashFlowNumber){ _cashFlowNumber = cashFlowNumber; }

		//Methods		
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
		vector<cashflow> _cashflowVector;
		int _couponFreq;
		enums::DayCountEnum _dayCount;
		int _cashFlowNumber;
	};
}
#endif