//created by Wang Jianwei on 1 Dec 2012

#include "DiscountCurve.h"
#include <cmath>
#include "dateUtil.h"
#include "Enums.h"

using namespace utilities;
using namespace std;

typedef tuple<date, double> point;
typedef AbstractCurve<date> super;

double DiscountCurve::getDiscountFactor(date date0){
	return getValue(date0);
}

double DiscountCurve::getFLiborRate(date forwardStartDate,date forwardEndDate,enums::DayCountEnum dayCount) {
	double cal=getDiscountFactor(forwardStartDate)/getDiscountFactor(forwardEndDate);

	return (cal-1)/dateUtil::getAccrualFactor(forwardStartDate,forwardEndDate,dayCount);
}

std::string DiscountCurve::toString(){
	return "\nDiscount "+AbstractCurve::toString();
}

std::string DiscountCurve::toString(int interval){
	return "\nDiscount "+AbstractCurve::toString(interval);
}