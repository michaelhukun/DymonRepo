//created by Wang Jianwei on 01 Dec 2012
#ifndef RATECONVERTER_H
#define RATECONVERTER_H
#include "Enums.h"
#include "date.h"
#include <string>
#include <tuple>

using namespace enums;
using namespace std;
using namespace utilities;

namespace utilities{
	class RateConverter{
		
	typedef tuple<date, double> Point;

	public:

		RateConverter(){};
		RateConverter(date startDate, enums::DayCountEnum dayCount){
			_startDate = startDate; 
			_dayCount = dayCount;
		};
		~RateConverter(){};
		
		Point convertRate(Point point, enums::RateType originalRateType, enum::RateType targetRateType);
		double convertRate(date endDate, double rate, enums::RateType originalRateType, enum::RateType targetRateType);

	private:

		date _startDate;
		date _endDate;
		enums::DayCountEnum _dayCount;
		double zeroDF(double rate);
		double DFzero(double rate);
	};
}
#endif