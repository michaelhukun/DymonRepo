//created by Hu Kun on 16 Dec 2012
#ifndef BONDRICER_H
#define BONDRICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "BondCurve.h"
#include "cashflowLeg.h"

using namespace std;
using namespace instruments;

namespace instruments {
	class BondPricer: public AbstractPricer {
	
	public:
		
		BondPricer(){};
		~BondPricer(){};
								
		virtual double getMPV(cashflowLeg* couponLeg,BondCurve* bondCurve);
		
		virtual double getParYield(cashflowLeg* couponLeg,BondCurve* bondCurve);

	private: 
		cashflowLeg* _couponLeg;
		BondCurve* _bondCurve;	
	};
}
#endif