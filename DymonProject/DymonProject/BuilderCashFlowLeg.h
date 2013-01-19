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
		// buildDirection: 1=build from issueDate towards maturityDate
		// buildDirection: -1=build from maturityDate towards issueDate
		BuilderCashFlowLeg(){};
		~BuilderCashFlowLeg(){};
		
		//for Fixed Legs
		BuilderCashFlowLeg(enums::Instrument instrument, date issueDate, date maturityDatee, int tenorNumOfMonths,double couponRate,double notional, int paymentFreq, enums::MarketEnum market, int buildDirection);
		BuilderCashFlowLeg(enums::Instrument instrument, date issueDate, int tenorNumOfMonths,double couponRate,double notional, int paymentFreq, enums::MarketEnum market);
		
		//for Floating Legs
		BuilderCashFlowLeg(enums::Instrument instrument, date issueDate, date maturityDatee, int tenorNumOfMonths, DiscountCurve* yc,double notional, int paymentFreq, enums::MarketEnum market, int buildDirection);
		BuilderCashFlowLeg(enums::Instrument instrument, date issueDate, int tenorNumOfMonths,DiscountCurve* yc,double notional, int paymentFreq, enums::MarketEnum market);

		cashflowLeg* getCashFlowLeg(){
			return _cashflowLeg;
		}
		
	private:
		cashflowLeg* _cashflowLeg;		
	};
}
#endif