//created by Jianwei on 30 Nov 2012
#ifndef FXEUROPEANOPTION_H
#define FXEUROPEANOPTION_H
#include "OptionPricer.h"
#include "EuropeanOption.h"
#include "CcyPair.h"

using namespace instruments;

namespace instruments {
	class FXEuropeanOption:  public EuropeanOption{
	public:
		// Constructors
		FXEuropeanOption():EuropeanOption(){};
		~FXEuropeanOption(){};
		FXEuropeanOption(std::string ccyPairStr, date tradeDate, date expiryDate, date deliveryDate, VolType volType, double vol):
		EuropeanOption(tradeDate, expiryDate, deliveryDate, volType, NaN, NaN, vol){
			setCcyPair(CcyPair(ccyPairStr));
		}

		// Getters and Setters
		CcyPair* getCcyPair(){ return &_ccyPair; }
		double getDelta(){ return _delta; }
		double getTenorExpiry(){ return _tenorExpiry; }
		double getTenorDiscount(){ return _tenorDiscount; }
		int getDaysToExpiry(){ return _daysToExpiry; }

		void setCcyPair(CcyPair ccyPair){ _ccyPair = ccyPair; }
		void setDelta(double delta){ _delta = delta; }
		void setTenorExpiry(double tenorExpiry){ _tenorExpiry = tenorExpiry; }
		void setTenorDiscount(double tenorDiscount){ _tenorDiscount = tenorDiscount; }
		void setDaysToExpiry(int daysToExpiry){ _daysToExpiry = daysToExpiry; }

		// Methods
		double getMPV(){return 0;};
		std::string toString(){return "";}

	private:
		CcyPair _ccyPair;
		int _daysToExpiry;
		double _delta;
		double _tenorExpiry;
		double _tenorDiscount;
	};
}
#endif