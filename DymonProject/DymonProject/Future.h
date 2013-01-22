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
		Market getMarket(){ return _market; }
		std::string getName(){ return _name; }
		int getTenor(){ return _tenorInMonths; }
		double getQuotedPrice(){ return _quotedPrice; }
		enums::DayCountEnum getDayCount(){ return _dayCount; }

		// Setters
		void setMarket(Market market){ _market = market; }
		void setName(std::string name){ _name = name; }
		void setTenor(int tenorInMonths){ _tenorInMonths = tenorInMonths;}		
		void setQuotedPrice(double quotedPrice){ _quotedPrice = quotedPrice;}		
		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}

		// Methods
		virtual double getMPV(DiscountCurve* dc);

	private:

		Market _market;
		std::string _name;
		int _tenorInMonths;
		double _quotedPrice;
		enums::DayCountEnum _dayCount;
		Bond most
	};

}
#endif