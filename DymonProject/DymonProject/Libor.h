// Written by Jianwei on 23/2/2012

#ifndef LIBOR_H
#define LIBOR_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "Deposit.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

namespace instruments {
	class Libor: public Deposit{
	public:
		Libor():Deposit(){ _instrumentEnum = enums::LIBOR; }
		~Libor(){};
	
		// Getters and Setters

		double getLiborRate(){ return _rate; }
		int getTenorNum(){ return _tenorNum; }
		dateUtil::DateUnit getDateUnit(){ return _dateUnit; }
		enums::CurrencyEnum getCountry(){ return _country; }

		void setLiborRate(double liborRate){ _rate= liborRate; }
		void setTenorNum(int tenorNum){ _tenorNum = tenorNum; }
		void setDateUnit(dateUtil::DateUnit dateUnit){ _dateUnit = dateUnit; }
		void setCountry(enums::CurrencyEnum country){ _country = country; }

		// Methods
		void deriveDates();
		std::string toString(){return "";}

	private:
		int _tenorNum;
		dateUtil::DateUnit _dateUnit;
		enums::CurrencyEnum _country;
	};

}
#endif