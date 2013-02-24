//created by Hu Kun on 30 Nov 2012
//class for standard IRS with fix to floating legs in one Market.
//rates for fixed and floating in private data members are all in terms of annual rates
//rewrote again with cashflow constructs by Kun 16 Dec 2012
//revamped by Jianwei on 23/2/2012

#ifndef SWAP_H
#define SWAP_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "BuilderCashFlowLeg.h"
#include "DiscountCurve.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

namespace instruments {
	class Swap: public AbstractInstrument{
	public:
		Swap():AbstractInstrument(){
			_fixCashflowLeg = new cashflowLeg();
			_floatingCashflowLeg = new cashflowLeg();
		};
		~Swap(){};
		Swap(date tradeDate, date maturityDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market market, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection);
		Swap(date tradeDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market market, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates);

		// Getters and Setters

		int getPaymentFreqFixLeg() { return _paymentFreqFixLeg; }
		int getPaymentFreqFloatingLeg() { return _paymentFreqFloatingLeg; }
		cashflowLeg* getCashFlowVectorFix() { return _fixCashflowLeg; }
		cashflowLeg* getCashFlowVectorFloat() {	return _floatingCashflowLeg; }
		DiscountCurve* Swap::getDiscountCurve() { return _yc; }
		int getTenor(){ return _tenorNumOfMonths;}
		double getSwapRate(){ return _swapRate; }
		double getDaysToMty(){ return _daysToMty; }
		enums::DayCountEnum getDayCountFixed(){ return _fixCashflowLeg->getDayCount(); }
		enums::DayCountEnum getDayCountFloat(){ return _floatingCashflowLeg->getDayCount(); }

		void setSwapRate(double swapRate){ _swapRate= swapRate; }
		void setDaysToMty(int daysToMty){ _daysToMty = daysToMty; }
		void setDayCountFixed(enums::DayCountEnum dayCountFixed){ _fixCashflowLeg->setDayCount(dayCountFixed);}
		void setDayCountFloat(enums::DayCountEnum dayCountFloat){ _floatingCashflowLeg->setDayCount(dayCountFloat);}

		// Methods
		void deriveDates();
		void printCashflowLegFix();
		void printCashflowLegFloat();
		std::string toString(){return "";}

	private:

		cashflowLeg* _fixCashflowLeg;
		cashflowLeg* _floatingCashflowLeg;
		DiscountCurve* _yc;
		int _paymentFreqFixLeg;
		int _paymentFreqFloatingLeg;
		int _tenorNumOfMonths;
		double _swapRate;
		int _daysToMty;
	};

}
#endif