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

		OptionPricer(AbstractOption* option):AbstractPricer(){
			_option = option;
		};
		~OptionPricer(){};

		void init(Configuration*){}

		virtual double getMPV();

		double blackScholesFormula(enums::VolType VolTypeFlag, double S, double K, double vol, double r, double T);
		double blackFormula(enums::VolType VolTypeFlag, double FwdS, double K, double vol, double discountFactor, double T);		
		double getImpliedVolBlackATM(enums::VolType VolTypeFlag, double K,  double optionPrice, double discountFactor, double T);

		double deriveDelta(double vol);


	protected:
		AbstractOption* _option;
	};
}
#endif