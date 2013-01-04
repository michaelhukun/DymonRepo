//created by Hu Kun Dec 13

#ifndef BUILDERCASHFLOWLEG_H
#define BUILDERCASHFLOWLEG_H

#include "cashflow.h"
#include "Market.h"
#include "cashflowLeg.h"
#include "Enums.h"
#include "date.h"
#include "dateUtil.h"
#include <vector>
#include <iterator>
#include "RecordHelper.h"
#include "DiscountCurve.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace Session;
using namespace instruments;

namespace instruments {
	class BuilderCashFlowLeg {
		
	public:
		// buildDirection: 1=build from startDate towards maturityDate
		// buildDirection: -1=build from maturityDate towards startDate
		BuilderCashFlowLeg(){};
		
		//for Fixed Legs
		BuilderCashFlowLeg(date startDate, date maturityDate,double couponRate,double notional, int paymentFreq, enums::MarketEnum market, int buildDirection);
		BuilderCashFlowLeg(date startDate, int tenorNumOfMonths,double couponRate,double notional, int paymentFreq, enums::MarketEnum market);
		
		//for Floating Legs
		BuilderCashFlowLeg(date startDate, date maturityDate,DiscountCurve* yc,double notional, int paymentFreq, enums::MarketEnum market, int buildDirection);
		BuilderCashFlowLeg(date startDate, int tenorNumOfMonths,DiscountCurve* yc,double notional, int paymentFreq, enums::MarketEnum market);

		~BuilderCashFlowLeg(){};
		cashflowLeg* getCashFlowLeg(){return &_cashflowLeg;};
		
	private:
		cashflowLeg _cashflowLeg;
		date _startDate;
		date _maturityDate;
		int _tenorNumOfMonths;
		double _couponRate;
		double _notional;
		int _paymentFreq;
		Market _cashFlowLegCurr;
		bool _rollAccuralDates;
		RecordHelper::HolidayMap _holidayMap;		
	};
}
#endif