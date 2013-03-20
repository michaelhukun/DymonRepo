#include "DepositPricer.h"

using namespace instruments;

double DepositPricer::getMPV(){
	return 0;
}

double DepositPricer::deriveDepositRate(){
	double spotDateDF = _discountCurve->getValue(_deposit->getSpotDate());
	double deliveryDateDF = _discountCurve->getValue(_deposit->getDeliveryDate());
	double accrualFactor = dateUtil::getAccrualFactor(_deposit->getSpotDate(), _deposit->getExpiryDate(), _deposit->getDayCount());
	double rate = (spotDateDF/deliveryDateDF-1)/accrualFactor;
	return rate;
}

double DepositPricer::deriveForwardRate(){
	return deriveDepositRate();
}