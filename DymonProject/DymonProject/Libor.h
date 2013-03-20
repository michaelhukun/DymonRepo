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
		Libor():Deposit(){};
		~Libor(){};
	
		// Getters and Setters

		double getLiborRate(){ return _rate; }
		int getTenorInMonth(){ return _tenorInMonth; }

		void setLiborRate(double liborRate){ _rate= liborRate; }
		void setTenorInMonth(int tenorInMonth){ _tenorInMonth = tenorInMonth; }

		// Methods
		void deriveDates();
		std::string toString(){return "";}

	private:
		int _tenorInMonth;
	};

}
#endif