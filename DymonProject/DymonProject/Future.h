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
		enums::DayCountEnum getDayCount(){ return _dayCount; }

		// Setters
		void setTenor(int tenorInMonths){ _tenorInMonths = tenorInMonths;}		
		void setQuotedPrice(double quotedPrice){ _quotedPrice = quotedPrice;}		
		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}

		// Methods
		virtual double getMPV(DiscountCurve* dc){return 0;};

	private:

		int _tenorInMonths;
		double _quotedPrice;
		enums::DayCountEnum _dayCount;
	};

}
#endif