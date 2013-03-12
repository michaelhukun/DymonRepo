//created by Wang Jianwei on 1 Dec 2012

#include "FXForwardImpliedBootStrapper.h"
#include <iostream>
#include "marketdata.h"
#include "DiscountCurve.h"
#include "InterpolatorFactory.h"
#include "AbstractInterpolator.h"
#include "NumericalFactory.h"
#include "AbstractBootStrapper.h"
#include "dateUtil.h"
#include "RecordHelper.h"
#include "Constants.h"

using namespace utilities;
using namespace Markets;

typedef AbstractBootStrapper<date> super;

void FXForwardImpliedBootStrapper::init(Configuration* cfg){
	super::init(cfg);
}

AbstractInterpolator<date>* FXForwardImpliedBootStrapper::bootStrap(){
	AbstractInterpolator<date>* ai;
	double spot = _forward->getSpot();
	double forward = _forward->getOutRight();
	double discountFactor = 0;
	double baseYieldCurveCcyDF = getBaseYieldCurveCcyDF(_baseYieldCurveCcy);

	if (_forward->getCcyPair()->getCCY1Enum() == _baseYieldCurveCcy){
		discountFactor = spot*baseYieldCurveCcyDF/forward;
	} else {
		discountFactor = forward/spot*baseYieldCurveCcyDF;
	}

	ai = InterpolatorFactory<date>::getInstance()->getInterpolator(_startPoint, point(_endDate,discountFactor) , _interpolAlgo);
	return ai;
}

double FXForwardImpliedBootStrapper::getBaseYieldCurveCcyDF(enums::CurrencyEnum baseYieldCurveCcy){
	DiscountCurve* dc = MarketData::getInstance()->getSwapDiscountCurve(baseYieldCurveCcy);
	date spotDate = _forward->getSpotDate();
	date deliveryDate = _forward->getDeliveryDate();
	double discountRate = dc->getDiscountFactor(spotDate, deliveryDate);
	return discountRate;
}

double FXForwardImpliedBootStrapper::numericalFunc(double x){
	return 0;
}