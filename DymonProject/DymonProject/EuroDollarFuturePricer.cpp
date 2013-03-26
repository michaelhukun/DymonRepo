#include "EuroDollarFuturePricer.h"
#include "EuroDollarFuture.h"

using namespace instruments;

double EuroDollarFuturePricer::getMPV(){
	double forwardLiborRate = deriveForwardLibor();
	return (1-forwardLiborRate)*100;
}

double EuroDollarFuturePricer::deriveConvAdjFwdLibor(){
	EuroDollarFuture* euroDollarFuture = static_cast<EuroDollarFuture*>(_future);
	date interestStartDate = _future->getStartDate();
	date interestEndDate = euroDollarFuture->getMaxFutureAndResetDeliveryDate();
	double interestStartDateDF = _discountCurve->getValue(interestStartDate);
	double interestEndDateDF = _discountCurve->getValue(interestEndDate);
	double accrualFactor = dateUtil::getAccrualFactor(interestStartDate, interestEndDate, _future->getDayCount());
	double forwardLiborRate = (interestStartDateDF/interestEndDateDF-1)/accrualFactor;
	return forwardLiborRate;
}

double EuroDollarFuturePricer::deriveForwardLibor(){
	EuroDollarFuture* euroDollarFuture = static_cast<EuroDollarFuture*>(_future);
	double forwardLiborRate = deriveConvAdjFwdLibor();
	return forwardLiborRate+euroDollarFuture->getConvAdj();
}