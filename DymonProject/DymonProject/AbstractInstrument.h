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
		virtual Market getMarket(){ return _market; }
		virtual date getIssueDate() { return _issueDate; }
		virtual date getTradeDate() { return _tradeDate; }
		virtual date getSpotDate() { return _spotDate; }
		virtual date getExpiryDate() { return _expiryDate; }
		virtual date getDeliveryDate() { return _deliveryDate; }
		virtual std::string getID(){ return _ID; }
		virtual std::string getName(){ return _name; }
		virtual std::string getTenorStr(){ return _tenorStr; }
		virtual enums::DayCountEnum getDayCount(){ return _dayCount; }
		
		// Setters
		virtual void setMarket(Market market){ _market = market; }
		virtual void setIssueDate(date issueDate) { _issueDate=issueDate; }
		virtual void setTradeDate(date tradeDate) { _tradeDate=tradeDate; }
		virtual void setSpotDate(date spotDate){ _spotDate = spotDate; }
		virtual void setExpiryDate(date expiryDate){ _expiryDate = expiryDate; }
		virtual void setDeliveryDate(date deliveryDate) { _deliveryDate=deliveryDate; }
		virtual void setName(std::string name){ _name = name; }
		virtual void setID(std::string ID){ _ID = ID;}
		virtual void setTenorStr(std::string tenorStr){ _tenorStr = tenorStr; }
		virtual void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}

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
		std::string _tenorStr;
		enums::DayCountEnum _dayCount;
	};
}
#endif