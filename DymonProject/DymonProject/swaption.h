//created by Hu Kun on 30 Nov 2012

#ifndef SWAPTION_H
#define SWAPTION_H
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
#include "Swap.h"
#include "AbstractOption.h"
#include "SwaptionVolSurface.h"
#include "SwaptionVolCube.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;


namespace instruments {
	class Swaption:  public Swap, public AbstractOption {
		
		Swaption(){};
		~Swaption(){};
		Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double K, SwaptionVolCube* vs, date swapStartDate, int tenorNumOfMonths,double notional, double couponRate, DiscountCurve* yc, Market fixLegCurr, Market floatingLegCurr, int paymentFreqFixLeg, int paymentFreqFloatingLeg, bool rollAccuralDates, int buildDirection);
		Swaption(Market market,PayReceive PayReceiveInd, int expiryInMonth, double K, SwaptionVolCube* vs, DiscountCurve* yc, Swap* underlyingSwap);
		
		Swap* getSwap(){ return _underlyingSwap; }

		virtual double getMPV();

	private:		

		int _tenorInMonth;
		Swap* _underlyingSwap;

	};
}
#endif