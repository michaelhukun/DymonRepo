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
		// buildDirection: 1=build from accrualStartDate towards accrualEndDate
		// buildDirection: -1=build from accrualEndDate towards accrualStartDate
		BuilderCashFlowLeg(){};
		
		//for Fixed Legs
		BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, date accrualEndDate,double couponRate,double notional, int paymentFreq, enums::MarketEnum market, int buildDirection);
		BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, int tenorNumOfMonths,double couponRate,double notional, int paymentFreq, enums::MarketEnum market);
		
		//for Floating Legs
		BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, date accrualEndDate,DiscountCurve* yc,double notional, int paymentFreq, enums::MarketEnum market, int buildDirection);
		BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, int tenorNumOfMonths,DiscountCurve* yc,double notional, int paymentFreq, enums::MarketEnum market);

		~BuilderCashFlowLeg(){};
		cashflowLeg* getCashFlowLeg(){return &_cashflowLeg;};
		
	private:
		cashflowLeg _cashflowLeg;
		date _accrualStartDate;
		date _accrualEndDate;
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