//created by Jianwei on 30 Nov 2012
#ifndef BOND_H
#define BOND_H
#include "AbstractInstrument.h"
#include "Enums.h"
#include "Market.h"
#include "BondCurve.h"
#include "BondPricer.h"

using namespace utilities;
using namespace instruments;

namespace instruments {
	class  Bond:  public BondPricer, public AbstractInstrument{

	public:

		Bond(){};
		~Bond(){};
		Bond(Market market, date tradeDate, date maturityDate, double notional, double couponRate, BondCurve* yc, int couponFreq, bool rollAccuralDates, int buildDirection);
		Bond(Market market, date tradeDate, int tenorNumOfMonths, double notional, double couponRate, BondCurve* yc, int couponFreq, bool rollAccuralDates);

		cashflowLeg* getCouponLeg();
		BondCurve* getBondCurve();
		int getCouponFreq();
		int getTenor(){ return _tenorNumOfMonths;}

		void printCouponLeg();

	private:

		cashflowLeg* _couponLeg;
		BondCurve* _bc;
		Market _market;
		int _couponFreq;
		int _tenorNumOfMonths;
	};

}
#endif