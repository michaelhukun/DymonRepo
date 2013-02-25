//created by Hu Kun on 30 Nov 2012

#ifndef SWAPTION_H
#define SWAPTION_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include <vector>
#include <tuple>
#include <iterator>
#include "cashflow.h"
#include "cashflowLeg.h"
#include "CashFlowLegBuilder.h"
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
	class Swaption: public AbstractOption {
		
	public:
		Swaption(){};
		~Swaption(){};
		Swaption(Market market, PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, SwaptionVolCube* vc, DiscountCurve* dc, Swap* underlyingSwap);
		Swaption(Market market, PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, Swap* underlyingSwap);
		Swaption(Market market, PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, int tenorInMonth);

		Swap* getSwap(){ return _underlyingSwap; }

		virtual double getAnnuityMonetizer( DiscountCurve* dc);
		virtual double getMPV();
		std::string toString(){return "";}

	private:		

		int _tenorInMonth;
		Swap* _underlyingSwap;		

		void BaseSwaption(Market market, PayReceive PayReceiveInd, int expiryInMonth, double strikeInBps, SwaptionVolCube* vc, DiscountCurve* dc, Swap* underlyingSwap);
	};
}
#endif