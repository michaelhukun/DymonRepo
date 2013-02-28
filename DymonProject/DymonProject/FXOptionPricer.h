//created by Jianwei on 30 Nov 2012
#ifndef FXOPTIONPRICER_H
#define FXOPTIONPRICER_H
#include "OptionPricer.h"
#include "FXEuropeanOption.h"

using namespace utilities;
using namespace instruments;

namespace instruments {
	
	class FXOptionPricer: OptionPricer{
	
	public:
		FXOptionPricer():OptionPricer(){};
      FXOptionPricer(FXEuropeanOption* option):OptionPricer(option){};
		~FXOptionPricer(){};		
		
      void init(Configuration*){}

		double getMPV();
      double deriveD1();
		double deriveDelta();
		
	};
}
#endif