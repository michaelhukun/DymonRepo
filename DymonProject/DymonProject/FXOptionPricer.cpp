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
   double BSdelta = option->getDiscountFactor() * MathUtil::CNF(d1);
   double PremDelta = option->getStrike()/option->getPrice() * BSdelta;
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