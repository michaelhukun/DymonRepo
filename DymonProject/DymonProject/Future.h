//created by Jianwei on 30 Nov 2012
#ifndef FUTURE_H
#define FUTURE_H
#include "AbstractInstrument.h"
#include "Enums.h"
#include "Market.h"
#include "DiscountCurve.h"
#include "Configuration.h"
#include <iostream>

using namespace utilities;
using namespace instruments;
using namespace std;

namespace instruments {
	class  Future:  public AbstractInstrument{

	public:

		Future(){};
		~Future(){};

		// Getters
		int getTenor(){ return _tenorInMonths; }
		double getQuotedPrice(){ return _quotedPrice; }
		double getSpotPrice(){ return _spotPrice; }

		// Setters
		void setTenor(int tenorInMonths){ _tenorInMonths = tenorInMonths;}		
		void setQuotedPrice(double quotedPrice){ _quotedPrice = quotedPrice;}		
		void setSpotPrice(double spotPrice){ _spotPrice = spotPrice; }

		// Methods
		virtual double getMPV(double costOfCarryRate);

	protected:

		int _tenorInMonths;
		double _quotedPrice;
		double _spotPrice;
	};

}
#endif