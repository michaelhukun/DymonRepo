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
		Deposit():AbstractInstrument(){};
		~Deposit(){};
	
		// Getters and Setters

		double getDepositRate(){ return _depositRate; }
		double getDaysToMty(){ return _daysToMty; }
		bool getIsOverNight(){ return _isOverNight; }

		void setDepositRate(double depositRate){ _depositRate= depositRate; }
		void setDaysToMty(int daysToMty){ _daysToMty = daysToMty; }
		void setIsOverNight(bool isOverNight){ _isOverNight = isOverNight; }

		// Methods
		
		void deriveAccrualStartDate();
		std::string toString(){return "";}

	private:


		double _depositRate;
		int _daysToMty;
		bool _isOverNight;
	};

}
#endif