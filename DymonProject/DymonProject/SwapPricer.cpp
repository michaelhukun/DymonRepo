//created by Hu Kun on 16 Dec 2012
#include "SwapPricer.h"
#include "AbstractPricer.h"
#include "cashflowLeg.h"
#include "DepositPricer.h"
#include "Libor.h"

using namespace std;
using namespace instruments;

double SwapPricer::getFixLegPV() {
	vector<cashflow> cfVector=_swap->getCashFlowLegFix()->getCashFlowVector();
	double mpv=0;
	for (auto it=cfVector.begin();it!=cfVector.end();it++) {
		cashflow cf=*it;
		double discountFactor = _discountCurve->getDiscountFactor(cf.getPaymentDate());
		mpv += cf.getAccuralFactor() * discountFactor;
		cout.precision(12);
		//cout<<"Fixed Leg: PaymentDate "<<paymentDate.toString()<<", DF " << discountFactor<<endl;
	}

	return mpv;
}

double SwapPricer::getFloatLegPV() {
	vector<cashflow> cfVector = _swap->getCashFlowLegFloat()->getCashFlowVector();
	double mpv=0;

	for(unsigned int i=0; i<cfVector.size(); i++){
		cashflow* cf = &cfVector.at(i);
		Libor* libor = (Libor*) cf->getReset();
		DepositPricer pricer(libor);
		pricer.setDiscountCurve(_discountCurve);
		double forwardRate = pricer.deriveForwardRate();
		mpv = mpv + forwardRate * cf->getAccuralFactor() * _discountCurve->getDiscountFactor(cf->getPaymentDate());
	}
	return mpv;
}

double SwapPricer::getMPV(){
	double mpv = getFixLegPV() * _swap->getSwapRate() - getFloatLegPV();
	return mpv;
}

double SwapPricer::deriveSwapRate() {
	double swapRate = getFloatLegPV() / getFixLegPV();
	return swapRate;
}

