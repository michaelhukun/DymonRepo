//created by Wang Jianwei on 1 Dec 2012

#include "DiscountCurve.h"
#include <cmath>
#include "dateUtil.h"
#include "Enums.h"

using namespace utilities;
using namespace std;

typedef tuple<date, double> point;
typedef AbstractCurve<date> super;

double DiscountCurve::getDiscountFactor(date endDate){
	date beginDate = get<0>(super::getCurveStartPoint());
	return getDiscountFactor(beginDate, endDate);
}

double DiscountCurve::getDiscountFactor(date beginDate, date endDate){
	double startDCF = getValue(beginDate);
	double endDCF = getValue(endDate);
	return endDCF/startDCF;
}

double DiscountCurve::getDiscountFactor(date endDate, double zeroRate){
	date beginDate = get<0>(super::getCurveStartPoint());
	double dayCountFraction = dateUtil::getAccrualFactor(beginDate,endDate,_dayCount);
	double discountFactor = pow(1+zeroRate, -dayCountFraction);
	return discountFactor;
}

double DiscountCurve::getZeroRate(date endDate){
	return getZeroRate(endDate, _dayCount);
}

double DiscountCurve::getZeroRate(date endDate, double discountFactor){
	date beginDate = get<0>(super::getCurveStartPoint());
	return getZeroRate(beginDate, endDate, discountFactor, _dayCount);
}

double DiscountCurve::getZeroRate(date endDate, enums::DayCountEnum dayCount){
	date beginDate = get<0>(super::getCurveStartPoint());
	return getZeroRate(beginDate, endDate, dayCount);
}

double DiscountCurve::getZeroRate(date beginDate, date endDate, enums::DayCountEnum dayCount){
	double discountFactor = getDiscountFactor(beginDate, endDate);
	return getZeroRate(beginDate, endDate, discountFactor, dayCount);
}

double DiscountCurve::getZeroRate(date beginDate, date endDate, double discountFactor, enums::DayCountEnum dayCount){
	double dayCountFraction = dateUtil::getAccrualFactor(beginDate,endDate,dayCount);
	double zeroRate = pow(discountFactor, -1/dayCountFraction)-1;
	return zeroRate;
}

double DiscountCurve::getDFChangingZeroRate(date aDate, double zeroRateDiff, enums::DayCountEnum dayCount){
	double zeroRate = getZeroRate(aDate, dayCount);
	double modifiedZeroRate = zeroRate + zeroRateDiff;
	double dayCountFraction = dateUtil::getAccrualFactor(dateUtil::getToday(),aDate,dayCount);
	double discountFactor = pow(1+modifiedZeroRate, dayCountFraction);
	return discountFactor;
}

double DiscountCurve::getFLiborRate(date forwardStartDate,date forwardEndDate,enums::DayCountEnum dayCount) {
	double cal=getDiscountFactor(forwardStartDate)/getDiscountFactor(forwardEndDate);
	return (cal-1)/dateUtil::getAccrualFactor(forwardStartDate,forwardEndDate,dayCount);
}