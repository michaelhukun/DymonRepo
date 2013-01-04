//created by Hu Kun on 30 Nov 2012
//class for zero coup bonds in one Market.

#ifndef ZERO_H
#define ZERO_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include <vector>

using namespace utilities;
using namespace std;

namespace instruments {
	class zero: public AbstractInstrument {

	public:
		zero();
		zero(double spotRate,double notional,date maturityDate, date issueDate);
		~zero();
		double getPrice();
		double getImpliedSpotRate();
		//double getPriceWrapper();
		double getImpliedSpotRateWrapper();

		virtual utilities::date getIssueDate();
	    virtual utilities::date getMaturityDate();
		virtual void setIssueDate(utilities::date issueDate);
		virtual void setMaturityDate(utilities::date maturityDate);

	public:
		double spotRate;
		double notional;
		date maturityDate;
		double price;
		date issueDate;
		date todayDate;


	};

}

#endif