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
			_fixedCashflowLeg = new cashflowLeg();
			_floatCashflowLeg = new cashflowLeg();
		};
		~Swap(){};
		Swap(date tradeDate, date maturityDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market market, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection);
		Swap(date tradeDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market market, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates);

		// Getters and Setters

		int getPayFreqFixed() { return _fixedCashflowLeg->getCouponFreq(); }
		int getPayFreqFloat() { return _floatCashflowLeg->getCouponFreq(); }
		cashflowLeg* getCashFlowVectorFix() { return _fixedCashflowLeg; }
		cashflowLeg* getCashFlowVectorFloat() {	return _floatCashflowLeg; }
		DiscountCurve* Swap::getDiscountCurve() { return _yc; }
		int getTenor(){ return _tenorNumOfMonths;}
		double getSwapRate(){ return _swapRate; }
		double getDaysToMty(){ return _daysToMty; }
		enums::DayCountEnum getDayCountFixed(){ return _fixedCashflowLeg->getDayCount(); }
		enums::DayCountEnum getDayCountFloat(){ return _floatCashflowLeg->getDayCount(); }

		void setSwapRate(double swapRate){ _swapRate= swapRate; }
		void setDaysToMty(int daysToMty){ _daysToMty = daysToMty; }
		void setDayCountFixed(enums::DayCountEnum dayCountFixed){ _fixedCashflowLeg->setDayCount(dayCountFixed);}
		void setDayCountFloat(enums::DayCountEnum dayCountFloat){ _floatCashflowLeg->setDayCount(dayCountFloat);}
		void setPayFreqFixed(int payFreqFixed){ _fixedCashflowLeg->setCouponFreq(payFreqFixed);}
		void setPayFreqFloat(int payFreqFloat){ _floatCashflowLeg->setCouponFreq(payFreqFloat);}


		// Methods
		void deriveDates();
		void buildFixedLeg();
		void buildFloatLeg();
		void printCashflowLegFix();
		void printCashflowLegFloat();
		std::string toString(){return "";}

	private:

		cashflowLeg* _fixedCashflowLeg;
		cashflowLeg* _floatCashflowLeg;
		DiscountCurve* _yc;
		int _tenorNumOfMonths;
		double _swapRate;
		int _daysToMty;
	};

}
#endif