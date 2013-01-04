//created by Hu Kun on 16 Dec 2012
#ifndef SWAPPRICER_H
#define SWAPPRICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"

#include "cashflowLeg.h"

using namespace std;
using namespace instruments;


namespace instruments {
	class SwapPricer: public AbstractPricer {
	
	public:
		
		SwapPricer(){};
		~SwapPricer(){};
								
		virtual double getMPV(cashflowLeg* fixCashflowLeg,cashflowLeg* floatCashflowLeg,DiscountCurve* aDiscountCurve);

		//fixOrFloating=-1 ==>floating
		//fixOrFloating=1 ==>fixed

		//vector<PV> getPVLeg(instruments::swap aSwap,DiscountCurve aDiscountCurve,int fixOrFloating);
		
		virtual double getParRate(cashflowLeg* floatCashflowLeg,cashflowLeg* fixCashflowLeg,DiscountCurve* aDiscountCurve);

	protected:
		virtual double getMPVFixLeg(cashflowLeg* fixCashflowLeg,DiscountCurve* aDiscountCurve);
		virtual double getMPVFloatLeg(cashflowLeg* floatCashflowLeg,DiscountCurve* aDiscountCurve);
		virtual double calFLiborRate(date forwardStartDate, date forwardEndDate, double accuralFactor);

	private: 
		vector<PV> _PVFixLeg;
		vector<PV> _PVFloatingLeg;
		double _parRate;
		cashflowLeg* _fixCashflowLeg;
		cashflowLeg* _floatCashflowLeg;
		//vector<FWDR> _FLiborRate;

		//instruments::swap _swapToBePriced;
		DiscountCurve* _pricingDiscountCurve;

	
	};
}
#endif