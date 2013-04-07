#include "DepositPricer.h"

using namespace instruments;

double DepositPricer::getMPV(){
	return 0;
}

double DepositPricer::deriveDepositRate(){
	date spotDate = _deposit->getSpotDate();
	date deliveryDate = _deposit->getDeliveryDate();
	date expiryDate = _deposit->getExpiryDate();
	enums::DayCountEnum dayCount = _deposit->getDayCount();
	double spotDateDF = _discountCurve->getValue(spotDate);
	double deliveryDateDF = _discountCurve->getValue(deliveryDate);
	double accrualFactor = dateUtil::getAccrualFactor(spotDate, expiryDate, dayCount);
	double rate = (spotDateDF/deliveryDateDF-1)/accrualFactor;
	return rate;
}

double DepositPricer::deriveForwardRate(){
	return deriveDepositRate();
}