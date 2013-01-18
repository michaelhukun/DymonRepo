//created by Hu Kun on 16 Dec 2012
#ifndef BONDRICER_H
#define BONDRICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "DiscountCurve.h"
#include "cashflowLeg.h"

using namespace std;
using namespace instruments;

namespace instruments {
	class BondPricer: public AbstractPricer {
	
	public:
		
		BondPricer(){};
		~BondPricer(){};
						
		virtual double getMPV(){return 0;}

		virtual double getMPV(cashflowLeg* couponLeg,DiscountCurve* discountCurve);
		
		virtual double getParYield(cashflowLeg* couponLeg,DiscountCurve* discountCurve);
	};
}
#endif