//created by Hu Kun on 29 Nov 2012
#ifndef INSTRUMENTBASE_H
#define INSTRUMENTBASE_H
#include "currency.h"
#include "date.h"


namespace instruments {
	class instrumentBase {
	
	public:
		//instrumentBase();
		//instrumentBase(instruments::currency domCurrency, instruments::currency forCurrency, utilities::date issueDate, utilities::date maturityDate);
		//~instrumentBase();

		//base class for all other instruments to be derived from
		virtual utilities::date getIssueDate() =0;
	    virtual utilities::date getMaturityDate()=0;
		virtual void setIssueDate(utilities::date issueDate)=0;
		virtual void setMaturityDate(utilities::date maturityDate)=0;

	protected: 
		//instruments::currency domCurrency;
		//instruments::currency forCurrency;
		//utilities::date issueDate;
		//utilities::date maturityDate;

		
	};
}
#endif