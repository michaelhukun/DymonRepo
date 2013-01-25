#include "FuturePricer.h"
#include "dateUtil.h"


using namespace instruments;
using namespace utilities;

void FuturePricer::init(Configuration* cfg){
}

double FuturePricer::getMPV(double costOfCarryRate){
	int daysToMaturity = dateUtil::getDaysBetween(_future->getTradeDate(), _future->getMaturityDate());
	double discountFactor = exp(-costOfCarryRate*daysToMaturity/360);
	double MPV = _future->getSpotPrice()/discountFactor;
	return MPV;
}