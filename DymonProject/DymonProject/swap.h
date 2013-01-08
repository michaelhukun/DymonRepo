//created by Hu Kun on 30 Nov 2012
//class for standard IRS with fix to floating legs in one Market.
//rates for fixed and floating in private data members are all in terms of annual rates
//rewrote again with cashflow constructs by Kun 16 Dec 2012

#ifndef SWAP_H
#define SWAP_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include <vector>
#include "zero.h"
#include <tuple>
#include <iterator>
#include "cashflow.h"
#include "cashflowLeg.h"
#include "BuilderCashFlowLeg.h"
#include "RecordHelper.h"
#include "SwapPricer.h"
#include "AbstractPricer.h"
#include "DiscountCurve.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;


namespace instruments {
	class Swap:  public SwapPricer, public AbstractInstrument{
	public:
		Swap(){};
		~Swap(){};
		Swap(date tradeDate, date maturityDate, double notional, double couponRate, DiscountCurve* yc, Market fixLegCurr, Market floatingLegCurr, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection);
		Swap(date tradeDate, int tenorNumOfMonths, double notional, double couponRate, DiscountCurve* yc, Market fixLegCurr, Market floatingLegCurr, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates);

		cashflowLeg* getCashflowLegFix();
		cashflowLeg* getCashflowLegFloat();
		DiscountCurve* getDiscountCurve();
		Market getFixLegCurr();
		Market getFloatLegCurr();
		int getPaymentFreqFixLeg();
		int getPaymentFreqFloatingLeg();
		int getTenor(){ return _tenorNumOfMonths;}

		void printCashflowLegFix();
		void printCashflowLegFloat();

	private:

		cashflowLeg* _fixCashflowLeg;
		cashflowLeg* _floatingCashflowLeg;
		DiscountCurve* _yc;
		Market _fixLegCurr;
		Market _floatingLegCurr;
		int _paymentFreqFixLeg;
		int _paymentFreqFloatingLeg;
		int _tenorNumOfMonths;
	};

}
#endif