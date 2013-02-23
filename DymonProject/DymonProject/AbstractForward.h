//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTFORWARD_H
#define ABSTRACTFORWARD_H
#include "Enums.h"
#include "CcyPair.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "Market.h"

using namespace utilities;
using namespace instruments;

namespace utilities {

	class AbstractForward: public AbstractInstrument {

	public:
		AbstractForward(){};
		~AbstractForward(){};

		// Getters and Setters
		double getSpot(){ return _spot; }
		double getPoint(){ return _point; }
		double getOutRight(){ return _outRight; }
		int getDaysToMty(){ return _daysToMty; }

		void setSpot(double spot){ _spot = spot; }
		void setPoint(double point){ _point = point; }
		void setOutRight(double outRight){ _outRight = outRight;}
		void setDaysToMty(int daysToMty){ _daysToMty = daysToMty; }
		
		// Methods
		virtual std::string toString(){return "";};
	
	private:

		double _spot;
		double _point;
		double _outRight;
		int _daysToMty;
	
	};
}
#endif