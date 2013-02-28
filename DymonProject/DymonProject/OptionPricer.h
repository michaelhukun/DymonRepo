//created by Jianwei on 16 Dec 2012
//added analytical functions for getting ATM implied vol by Kun 27 Dec
#ifndef OPTIONPRICER_H
#define OPTIONPRICER_H
#include "AbstractPricer.h"
#include "AbstractOption.h"
#include "Enums.h"

using namespace instruments;
using namespace utilities;

namespace instruments {
	class OptionPricer: public AbstractPricer {

	public:

      OptionPricer():AbstractPricer(){}
		OptionPricer(AbstractOption* option):AbstractPricer(){
			_option = option;
		};
		~OptionPricer(){};

		void init(Configuration*){}

		virtual double getMPV();
		virtual double blackScholesFormula();
		virtual double blackFormula();		
		virtual double getImpliedVolBlackATM();
      virtual double deriveD1();
      virtual double deriveD2();
		virtual double deriveDelta();

	protected:
		AbstractOption* _option;
	};
}
#endif