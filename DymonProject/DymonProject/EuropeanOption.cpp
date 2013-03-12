//created by Jianwei 04 Dec 2012
#include "EuropeanOption.h"
#include "OptionPricer.h"

using namespace instruments;

double EuropeanOption::getMPV(){
   OptionPricer pricer(this);
   return pricer.blackScholesFormula();
}