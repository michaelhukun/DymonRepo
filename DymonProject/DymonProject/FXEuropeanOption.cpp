#include "FXEuropeanOption.h"
#include "marketdata.h"
#include "DiscountCurve.h"

using namespace instruments;
using namespace Markets;
using namespace utilities;

void FXEuropeanOption::deriveTenorExpiry(){
	int daysToExpiry = dateUtil::getDaysBetween(getTradeDate(), getExpiryDate());
	double tenorExpiry = daysToExpiry / numDaysInYear;
	setTenorExpiry(tenorExpiry);
}

void FXEuropeanOption::deriveDomesticDCF(){
	enums::CurrencyEnum domesticCcy = _ccyPair.getDomesticCCYEnum();
	DiscountCurve* domesticSwapCurve = MarketData::getInstance()->getSwapDiscountCurve(domesticCcy);
	_domesticDCF = domesticSwapCurve->getDiscountFactor(_spotDate, _deliveryDate);
}


void FXEuropeanOption::deriveForeignDCF(){
	int daysBeforeSwitchToSwapCurve=stoi(Configuration::getInstance()->getProperty("FXSkew.daysBeforeSwitchToSwapCurve", false, "365"));
	enums::CurrencyEnum foreignCcy = _ccyPair.getForeignCCYEnum();
	DiscountCurve* curve;
	if (_deliveryDate - _tradeDate>daysBeforeSwitchToSwapCurve){
		curve = MarketData::getInstance()->getFXForwardImpliedCurve(foreignCcy);
	} else {
		curve = MarketData::getInstance()->getSwapDiscountCurve(foreignCcy);
	}
	_foreignDCF = curve->getDiscountFactor(_spotDate, _deliveryDate);
}