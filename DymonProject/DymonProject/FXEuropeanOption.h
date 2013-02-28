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
		double getForeignDCF(){ return _foreignDCF; }
		double getDomesticDCF(){ return _domesticDCF; }

		void setCcyPair(CcyPair ccyPair){ _ccyPair = ccyPair; }

		// Methods
		void deriveTenorExpiry();
		void deriveDomesticDCF();
		void deriveForeignDCF();
      void deriveDeltaType();
		double getMPV(){return 0;};
		void deriveVol();
		std::string toString(){return "";}

	private:
		double numericalFunc(double vol);
		
		CcyPair _ccyPair;
		double _domesticDCF;
		double _foreignDCF;
	};
}
#endif