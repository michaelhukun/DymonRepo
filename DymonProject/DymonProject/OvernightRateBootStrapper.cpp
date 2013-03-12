//created by Wang Jianwei on 1 Dec 2012

#include "OvernightRateBootStrapper.h"
#include <iostream>
#include <cmath>
#include "DiscountCurve.h"
#include "InterpolatorFactory.h"
#include "AbstractInterpolator.h"
#include "NumericalFactory.h"
#include "AbstractBootStrapper.h"
#include "dateUtil.h"
#include "RecordHelper.h"

using namespace utilities;

typedef AbstractBootStrapper<date> super;

void OvernightRateBootStrapper::init(Configuration* cfg){
	super::init(cfg);
}

AbstractInterpolator<date>* OvernightRateBootStrapper::bootStrap(){
	AbstractInterpolator<date>* ai;
	enums::DayCountEnum dayCount = _deposit->getDayCount();
	date accuralStartDate = _deposit->getSpotDate();
	date accuralEndDate = _deposit->getExpiryDate();
	double rate = _deposit->getRate();
	double accrualFactor = dateUtil::getAccrualFactor(accuralStartDate, accuralEndDate, dayCount);
	double discountFactor = 1/(1+accrualFactor*rate)*getPreviousDF();
	ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	return ai;
}


double OvernightRateBootStrapper::getPreviousDF(){
	double previousDF = 1;
	try{
		previousDF = get<1>(_curve->getCurveEndPoint());
	}catch(...){
	}
	return previousDF;
}

	double OvernightRateBootStrapper::numericalFunc(double x){
		return 0;
}