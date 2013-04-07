// Written by Jianwei on 23/2/2012

#ifndef DEPOSIT_H
#define DEPOSIT_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

namespace instruments {
	class Deposit: public AbstractInstrument{
	public:
		Deposit():AbstractInstrument(){ _instrumentEnum = enums::DEPOSIT; }
		~Deposit(){};
	
		// Getters and Setters

		double getDepositRate(){ return _rate; }
		double getDaysToMty(){ return _daysToMty; }
		bool getIsOverNight(){ return _isOverNight; }

		void setDepositRate(double depositRate){ _rate= depositRate; }
		void setDaysToMty(int daysToMty){ _daysToMty = daysToMty; }
		void setIsOverNight(bool isOverNight){ _isOverNight = isOverNight; }

		// Methods
		
		virtual void deriveAccrualStartDate();
		virtual std::string toString(){return "";}

	protected:
		
		int _daysToMty;
		bool _isOverNight;
	};

}
#endif