#include "FXEuropeanOption.h"
#include "marketdata.h"
#include "DiscountCurve.h"
#include "OptionPricer.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"
#include "Configuration.h"

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

void FXEuropeanOption::deriveVol(){
	Configuration* cfg = Configuration::getInstance();
	int _iterateCount = std::stoi(cfg->getProperty("numerical.iteration",true,"50"));
	double _tolerance = std::stod(cfg->getProperty("numerical.tolerance",true,"0.0000001"));
	enums::NumericAlgo _numericAlgo=  EnumHelper::getNumericalAlgo(cfg->getProperty("FXOption.numerical",false,"BISECTION"));
	double lowerBound = 0;
	double upperBound = 200;
	AbstractNumerical<FXEuropeanOption>* an = NumericalFactory<FXEuropeanOption>::getInstance()->getNumerical(this,&FXEuropeanOption::numericalFunc,_numericAlgo);
	_vol = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
}

double FXEuropeanOption::numericalFunc(double vol){
	OptionPricer pricer(this);
	double delta = pricer.deriveDelta(vol);
	double volFromSurface = MarketData::getInstance()->getFXSkewSurface(_ccyPair.toString())->getValue(_daysToExpiry, delta);
	return volFromSurface - vol;
}
