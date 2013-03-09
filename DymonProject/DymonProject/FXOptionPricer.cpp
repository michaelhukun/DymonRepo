#include "FXOptionPricer.h"
#include "FXEuropeanOption.h"
#include "OptionPricer.h"
#include "MathUtil.h"

using namespace utilities;
using namespace instruments;

double FXOptionPricer::getMPV(){
   return 0;
}

double FXOptionPricer::deriveD1(){
   FXEuropeanOption * option = static_cast<FXEuropeanOption*>(_option);
   double S = option->getPrice();
   double K = option->getStrike();
   double rTDomestic = -log(option->getDomesticDCF());
   double rTForeign = -log(option->getForeignDCF());
   double vol = option->getVol();
   double T = option->getExpiryTenor();
   double d1=(log(S/K) + rTDomestic - rTForeign + pow(vol,2) * T/2)/(vol*sqrt(T));
   return d1;
}

double FXOptionPricer::deriveDelta(){
   FXEuropeanOption * option = static_cast<FXEuropeanOption*>(_option);
   double d1 = deriveD1();
   double d2 = deriveD2();

   //double BSdelta = option->getDiscountFactor() * MathUtil::CNF(d1);
   //double PremDelta = option->getStrike()/option->getPrice() * BSdelta;

   int sign=(option->getDeltaType() == enums::Call)? 1: -1;
   double BSdelta = sign * option->getForeignDCF() * MathUtil::CNF(sign * d1);
   double PremDelta = sign * option->getStrike()/option->getPrice() * option->getDomesticDCF() * MathUtil::CNF(sign * d2);
   switch(option->getDeltaType()){
   case enums::BS:
      return BSdelta;
   case enums::PREMIUM:
      return PremDelta;
   case enums::FWDBS:
      return BSdelta/option->getForeignDCF();
   case enums::FWDPREMIUM:
      return PremDelta/option->getForeignDCF();
   }
   throw "Delta enum not found!";
}

double OptionPricer::blackFormula(){
FXEuropeanOption * option = static_cast<FXEuropeanOption*>(_option);
   double d1 = deriveD1();
   double d2 = deriveD2();
   double dcf = option->getDomesticDCF();
   double K = option->getStrike();
   double S = option->getPrice();

   int sign=(option->getDeltaType() == enums::Call)? 1: -1;
   return sign * dcf * (S * MathUtil::CNF(sign * d1)-K * MathUtil::CNF(sign * d2));
}