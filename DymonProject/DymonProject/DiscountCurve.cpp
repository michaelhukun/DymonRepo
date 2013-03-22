//created by Wang Jianwei on 1 Dec 2012

#include "DiscountCurve.h"
#include <cmath>
#include "dateUtil.h"
#include "RateConverter.h"
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
	RateConverter rc(beginDate, _dayCount);
	double discountFactor = rc.convertRate(endDate, zeroRate, enums::ZERO, enums::DF);
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
	RateConverter rc(beginDate, _dayCount);
	double zeroRate = rc.convertRate(endDate, discountFactor, enums::DF, enums::ZERO);
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

void DiscountCurve::dumpComponentNames(){
		cout << _name + " - Components: \n";
		for (int i = 0; i<_components.size(); i++){
			cout << _components.at(i)->getName() << "; \n";
		}
		cout<<endl;
}