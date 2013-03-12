//created by Hu Kun on 8 Dec
//modified by Hu Kun on 10 Dec

#ifndef CASHFLOW_H
#define CASHFLOW_H

#include "Market.h"
#include "Enums.h"
#include "date.h"
#include "dateUtil.h"
#include <vector>
#include "Deposit.h"

using namespace utilities;
using namespace std;
using namespace enums;

namespace instruments {
	class cashflow {
		

	public:
		cashflow(){};
		cashflow(Deposit* deposit, bool isValid);
		cashflow(double couponRate,double notional, date fixingDate, date paymentDate, date accuralStartDate, date accuralEndDate, Market cashFlowCurr, bool isValid);
      cashflow(date fixingDate, date paymentDate,date accuralStartDate, date accuralEndDate, enums::DayCountEnum dayCount, Market cashFlowCurr, bool isValid);
		~cashflow(){};

		// Getters and Setters
		double getCouponRate();
		double getNotional();
		date getFixingDate();
		date getPaymentDate();
		date getAccuralStartDate();
		date getAccuralEndDate();
		Market getCashFlowCurr();
		double getAccuralFactor();
		enums::DayCountEnum getDayCount(){ return _dayCount; }
		bool getIsValid();

		void setNotional(double notional);
		void setCouponRate(double couponRate);
		void setFixingDate(date fixingDate);
		void setPaymentDate(date paymentDate);
		void setAccuralStartDate(date accuralStartDate);
		void setAccuralEndDate(date accuralEndDate);
		void setCashFlowCurr(Market cashFlowCurr);
		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount; }
		void setIsValid(bool isValid);

		// Methods
		void deriveAccuralFactor();
		bool isDateEqual(cashflow cf);
		void printCashFlow();
		
	private:
		double _couponRate;
		double _notional;
		enums::DayCountEnum _dayCount;

		date _fixingDate;
		date _paymentDate; 
		date _accuralStartDate;
		date _accuralEndDate;
		
		Market _cashFlowCurr;
		double _accuralFactor;
		bool _isValid;
	};
}
#endif