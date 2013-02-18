//created by Wang Jianwei on 01 Dec 2012
#ifndef FXFORWARD_H
#define FXFORWARD_H
#include "Enums.h"
#include "CcyPair.h"
#include "date.h"
#include "AbstractForward.h"

using namespace utilities;
using namespace instruments;

namespace utilities {

	class FXForward: public AbstractForward{

	public:
		FXForward():AbstractForward(){};
		FXForward(std::string ccyPairStr, std::string tenorStr, enums::DayCountEnum dayCount, double point){
			setCcyPair(CcyPair(ccyPairStr));
			setPoint(point);
			setDayCount(dayCount);

		}
		~FXForward(){};

		// Getters and Setters
		double getPoint(){ return _point; }
		enums::DayCountEnum getDayCount(){ return _dayCount; }
		double getTenorInYear(){ return _tenorInYear; }
		CcyPair getCcyPair(){ return _ccyPair; }

		void setPoint(double point){ _point = point; }
		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}
		void setTenorInYear(double tenorInYear){ _tenorInYear = tenorInYear; }
		void setCcyPair(CcyPair ccyPair){ _ccyPair = ccyPair; }

	private:

		double _point;
		enums::DayCountEnum _dayCount;
		double _tenorInYear;
		std::string _tenorStr;
		CcyPair _ccyPair;
	
		void deriveDates(std::string tenorStr);
	};
}
#endif