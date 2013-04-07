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
		virtual date getFixingDate() { return _fixingDate; }
		virtual date getIssueDate() { return _issueDate; }
		virtual date getStartDate() { return _startDate; }
		virtual date getTradeDate() { return _tradeDate; }
		virtual date getSpotDate() { return _spotDate; }
		virtual date getExpiryDate() { return _expiryDate; }
		virtual date getDeliveryDate() { return _deliveryDate; }
		virtual std::string getID(){ return _ID; }
		virtual std::string getName(){ return _name; }
		virtual std::string getTenorStr(){ return _tenorStr; }
		virtual enums::DayCountEnum getDayCount(){ return _dayCount; }
		virtual enums::DayRollEnum getDayRoll(){ return _dayRoll; }
		virtual enums::Instrument getInstrumentEnum(){ return _instrumentEnum; }
		virtual double getNotional(){ return _notional; }
		virtual double getRate(){ return _rate; } 
		
		// Setters
		virtual void setMarket(Market market){ _market = market; }
		virtual void setFixingDate(date fixingDate) { _fixingDate = fixingDate; }
		virtual void setStartDate(date startDate) { _startDate=startDate; }
		virtual void setIssueDate(date issueDate) { _issueDate=issueDate; }
		virtual void setTradeDate(date tradeDate) { _tradeDate=tradeDate; }
		virtual void setSpotDate(date spotDate){ _spotDate = spotDate; }
		virtual void setExpiryDate(date expiryDate){ _expiryDate = expiryDate; }
		virtual void setDeliveryDate(date deliveryDate) { _deliveryDate=deliveryDate; }
		virtual void setName(std::string name){ _name = name; }
		virtual void setID(std::string ID){ _ID = ID;}
		virtual void setTenorStr(std::string tenorStr){ _tenorStr = tenorStr; }
		virtual void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}
		virtual void setDayRoll(enums::DayRollEnum dayRoll){ _dayRoll = dayRoll;}
		virtual void setInstrumentEnum(enums::Instrument instrumentEnum){ _instrumentEnum = instrumentEnum; }
		virtual void setNotional(double notional){ _notional = notional; }
		virtual void setRate(double rate){ _rate = rate; }

		// Method

		virtual std::string toString(){return "";}

	protected: 
		Market _market;
		date _fixingDate;
		date _issueDate;
		date _startDate;
		date _tradeDate;
		date _spotDate;
		date _expiryDate;
		date _deliveryDate;
		double _notional;
		double _rate;
		std::string _ID;
		std::string _name;
		std::string _tenorStr;
		enums::DayCountEnum _dayCount;
		enums::DayRollEnum _dayRoll;
		enums::Instrument _instrumentEnum;
	};
}
#endif