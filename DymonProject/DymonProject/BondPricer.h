//created by Hu Kun on 16 Dec 2012
#ifndef BONDRICER_H
#define BONDRICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "DiscountCurve.h"
#include "cashflowLeg.h"
#include "Configuration.h"
#include "Bond.h"


using namespace std;
using namespace instruments;

namespace instruments {
	class BondPricer: public AbstractPricer {
	
	public:
		
		BondPricer(){};
		BondPricer(Bond* bond);
		~BondPricer(){};

		void init(Configuration* cfg);
						
		virtual double getMPV(DiscountCurve* discountCurve);	

		double getZeroRateSpread(double dirtyPrice);
		
		double getYieldByDirtyPrice(double dirtyPrice);
		
		double getYieldByZeroRate(double zeroRate);
		
		double getYieldByDiscountFactor(double discountFactor);

		double yieldSolverFunc(double yield);

		double curveBumpSolverFunc(double zeroRateSpread);

	private:

		double _tempTargetBondPrice;

		enums::NumericAlgo _numericAlgo; 

		double _tolerance;

		int _iterateCount;

		double _lowerBound;
		
		double _upperBound;

		Bond* _bond;
	};
}
#endif