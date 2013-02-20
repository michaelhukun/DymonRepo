//created by Jianwei 04 Dec 2012
//added analytical functions for getting ATM implied vol by Kun 27 Dec
#include "OptionPricer.h"
#include "Enums.h"
#include "MathUtil.h"

using namespace utilities;
using namespace instruments;

double OptionPricer::blackScholesFormula(enums::VolType VolTypeFlag, double S, double K, double vol, double r, double T){
	double d1, d2;
	
	d1=(log(S/K)+(r+vol*vol/2)*T)/(vol*sqrt(T));
	d2=d1-vol*sqrt(T);

	if (VolTypeFlag == enums::Call)
		return S *MathUtil::CNF(d1)-K * exp(-r*T)*MathUtil::CNF(d2);
	return K * exp(-r * T) * MathUtil::CNF(-d2) - S * MathUtil::CNF(-d1);
	
}

double OptionPricer::blackFormula(enums::VolType VolTypeFlag, double FwdS, double K, double vol, double discountFactor, double T){
	double d1, d2;

	d1=(log(FwdS/K)+vol*vol/2*T)/(vol*sqrt(T));
	d2=d1-vol*sqrt(T);

	if (VolTypeFlag == enums::Call)
		return discountFactor*(FwdS *MathUtil::CNF(d1)-K * MathUtil::CNF(d2));
	return discountFactor*(K * MathUtil::CNF(-d2) - FwdS * MathUtil::CNF(-d1));
}


double OptionPricer::getImpliedVolBlackATM(enums::VolType VolTypeFlag, double K,  double optionPrice, double discountFactor, double T) {
	int sig=(VolTypeFlag == enums::Call)? 1: -1;
	double n=optionPrice/2/discountFactor/K/sig+1/2;	
	return 2*MathUtil::invCNF(n)/sig;
}



