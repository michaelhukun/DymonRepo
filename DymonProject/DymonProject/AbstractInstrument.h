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
		date getIssueDate() { return _issueDate; }
		date getTradeDate() { return _tradeDate; }
		date getSpotDate() { return _spotDate; }
		date getExpiryDate() { return _expiryDate; }
		date getDeliveryDate() { return _deliveryDate; }
		std::string getID(){ return _ID; }
		std::string getName(){ return _name; }
		
		// Setters
		void setMarket(Market market){ _market = market; }
		void setIssueDate(date issueDate) { _issueDate=issueDate; }
		void setTradeDate(date tradeDate) { _tradeDate=tradeDate; }
		void setSpotDate(date spotDate){ _spotDate = spotDate; }
		void setExpiryDate(date expiryDate){ _expiryDate = expiryDate; }
		void setDeliveryDate(date deliveryDate) { _deliveryDate=deliveryDate; }
		void setName(std::string name){ _name = name; }
		void setID(std::string ID){ _ID = ID;}

		// Method

		virtual std::string toString()=0;

	protected: 
		Market _market;
		date _issueDate;
		date _tradeDate;
		date _spotDate;
		date _expiryDate;
		date _deliveryDate;
		std::string _ID;
		std::string _name;
	};
}
#endif