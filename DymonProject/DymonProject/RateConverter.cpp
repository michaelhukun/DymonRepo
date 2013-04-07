#include "RateConverter.h"
#include "Constants.h"
#include "dateUtil.h"

using namespace utilities;

RateConverter::Point RateConverter::convertRate(RateConverter::Point point, enums::RateType originalRateType, enum::RateType targetRateType){
	_endDate = std::get<0>(point);
	double rate = std::get<1>(point);
	double convertedRate = convertRate(_endDate, rate, originalRateType, targetRateType);
	Point resultPoint(_endDate, convertedRate);
	return resultPoint;
}

double RateConverter::convertRate(date endDate, double rate, enums::RateType originalRateType, enum::RateType targetRateType){
	double convertedRate = rate;
	_endDate = endDate;
	if(originalRateType == DF){
		if (targetRateType ==ZERO){
			convertedRate = DFzero(rate);
		}
	}else if (originalRateType ==ZERO){
		if (targetRateType ==DF){
			convertedRate = zeroDF(rate);
		}
	}
	return convertedRate;
}

double RateConverter::zeroDF(double rate){
	double dayCountFraction = dateUtil::getAccrualFactor(_startDate,_endDate, _dayCount);
	double discountFactor = pow(1 + rate, -dayCountFraction);
	return discountFactor;
}

double RateConverter::DFzero(double rate){
	double dayCountFraction = dateUtil::getAccrualFactor(_startDate,_endDate, _dayCount);
	double zeroRate = pow(rate, -1/dayCountFraction)-1;
	return zeroRate;
}