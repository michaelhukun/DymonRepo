//created by Hu Kun on 29 Nov 2012
#ifndef ABSTRACTINSTRUMENT_H
#define ABSTRACTINSTRUMENT_H
#include "Market.h"
#include "date.h"
#include <iostream>

using namespace utilities;
using namespace std;

namespace instruments {
	class AbstractInstrument {
	
	public:
		//base class for all other instruments to be derived from
		AbstractInstrument(){};
		
		// Getters
		Market getMarket(){ return _market; }
		date getTradeDate() { return _tradeDate; }
		date getIssueDate() { return _issueDate; }
		date getMaturityDate() { return _maturityDate; }
		std::string getID(){ return _ID; }
		std::string getName(){ return _name; }
		
		// Setters
		void setMarket(Market market){ _market = market; }
		void setTradeDate(date tradeDate) { _tradeDate=tradeDate; }
		void setIssueDate(date issueDate) { _issueDate=issueDate; }
		void setMaturityDate(date maturityDate) { _maturityDate=maturityDate; }
		void setName(std::string name){ _name = name; }
		void setID(std::string ID){ _ID = ID;}

	protected: 
		Market _market;
		date _tradeDate;
		date _issueDate;
		date _maturityDate;	
		std::string _ID;
		std::string _name;
	};
}
#endif