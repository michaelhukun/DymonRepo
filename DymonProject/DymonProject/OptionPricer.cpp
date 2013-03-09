//created by Jianwei 04 Dec 2012
//added analytical functions for getting ATM implied vol by Kun 27 Dec
#include "OptionPricer.h"
#include "Enums.h"
#include "MathUtil.h"
#include "AbstractOption.h"

using namespace utilities;
using namespace instruments;

double OptionPricer::getMPV(){
   return 0;
}

double OptionPricer::blackScholesFormula(){
   double d1 = deriveD1();
   double d2 = deriveD2();
   double dcf = _option->getDiscountFactor();
   double K = _option->getStrike();
   double S = _option->getPrice();

   int sign=(_option->getDeltaType() == enums::Call)? 1: -1;
   return sign * (S * MathUtil::CNF(sign * d1)-K * dcf * MathUtil::CNF(sign * d2));
}

double OptionPricer::blackFormula(){
   double d1 = deriveD1();
   double d2 = deriveD2();
   double dcf = _option->getDiscountFactor();
   double K = _option->getStrike();
   double S = _option->getPrice();

   int sign=(_option->getDeltaType() == enums::Call)? 1: -1;
   return sign * dcf * (S * MathUtil::CNF(sign * d1)-K * MathUtil::CNF(sign * d2));
}

double OptionPricer::getImpliedVolBlackATM() {
	int sign=(_option->getDeltaType() == enums::Call)? 1: -1;
	double n=0; //optionPrice/2/discountFactor/K/sign+1/2;	
	return 2*MathUtil::invCNF(n)/sign;
}

double OptionPricer::deriveD1(){
   double S = _option->getPrice();
   double K = _option->getStrike();
   double rT = -log(_option->getDiscountTenor());
   double vol = _option->getVol();
   double T = _option->getExpiryTenor();
	double d1=(log(S/K)+rT+pow(vol,2)*T/2)/(vol*sqrt(T));
   return d1;
}

double OptionPricer::deriveD2(){
   double d1=deriveD1();
   double d2=d1-_option->getVol()*sqrt(_option->getExpiryTenor());
   return d2;
}

double OptionPricer::deriveDelta(){
   double d1 = _option->getD1();
   double BSdelta = _option->getDiscountFactor() * MathUtil::CNF(d1);
   return BSdelta;
}