#include "Future.h"
#include "FuturePricer.h"

using namespace instruments;

double Future::getMPV(double costOfCarryRate){
	FuturePricer pricer(this);
	double MPV = pricer.getMPV(costOfCarryRate);
	return MPV;
}