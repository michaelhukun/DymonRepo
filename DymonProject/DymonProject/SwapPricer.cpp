//created by Hu Kun on 16 Dec 2012
#include "SwapPricer.h"
#include "AbstractPricer.h"
#include "cashflowLeg.h"

using namespace std;
using namespace instruments;

double SwapPricer::getFixLegPV() {
	vector<cashflow> cfVector=_swap->getCashFlowLegFix()->getCashFlowVector();
	double mpv=0;
	for (auto it=cfVector.begin();it!=cfVector.end();it++) {
		cashflow cf=*it;
		date paymentDate=cf.getPaymentDate();
		mpv+=cf.getAccuralFactor()*_discountCurve->getDiscountFactor(paymentDate);
	}

	return mpv;
}

double SwapPricer::getFloatLegPV() {
	vector<cashflow> cfVector = _swap->getCashFlowLegFloat()->getCashFlowVector();
	cashflow cf = cfVector.at(cfVector.size()-1);
	double mpv = _discountCurve->getDiscountFactor(_swap->getSpotDate()) - _discountCurve->getDiscountFactor(cf.getPaymentDate());
	return mpv;
}

double SwapPricer::getMPV(){
   return 0;
}

double SwapPricer::deriveSwapRate() {
	double swapRate = getFloatLegPV() / getFixLegPV();
	return swapRate;
}

