//created by Jianwei 04 Dec 2012
#include "EuropeanOption.h"
#include "OptionPricer.h"

using namespace instruments;

double EuropeanOption::getMPV(){
	return blackScholesFormula(_VolTypeFlag,_S,_K,_vol,_r,_expiryInMonth/12);
}