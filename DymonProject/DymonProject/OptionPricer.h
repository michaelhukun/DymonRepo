//created by Jianwei on 16 Dec 2012
//added analytical functions for getting ATM implied vol by Kun 27 Dec
#ifndef OPTIONPRICER_H
#define OPTIONPRICER_H
#include "AbstractPricer.h"
#include "Enums.h"

using namespace instruments;
using namespace utilities;

namespace instruments {
	class OptionPricer: public AbstractPricer {

	public:

		OptionPricer():AbstractPricer(){};
		~OptionPricer(){};

		virtual double getMPV(){return AbstractPricer::getMPV();}
		
	protected:

		double blackScholesFormula(enums::CallPut callPutFlag, double S, double K, double vol, double r, double T);
		double blackFormula(enums::CallPut callPutFlag, double FwdS, double K, double vol, double discountFactor, double T);
		
		double getImpliedVolBlackATM(enums::CallPut callPutFlag, double K,  double optionPrice, double discountFactor, double T);

	};
}
#endif