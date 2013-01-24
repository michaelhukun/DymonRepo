//created by Hu Kun 13 Dec

#include "cashflowLeg.h"
#include "cashflow.h"
#include <math.h>
#include <stdio.h>
#include <iterator>
#include <iostream>
#include <sstream>
#include "Constants.h"

using namespace utilities;
using namespace std;
using namespace enums;

namespace instruments {

	cashflowLeg::cashflowLeg(vector<cashflow> cashflowLeg, int couponFreq) {
		setCashFlowVector(cashflowLeg);
		_couponFreq = couponFreq;
	}

	cashflow cashflowLeg::getCashFlow(unsigned int index){
		if (index<0||index>=_cashflowVector.size())
			throw "Index out of range!";

		return _cashflowVector[index];
	}

	int cashflowLeg::getFirstValidCashFlowIndex(){
		for (unsigned int i=0;i<_cashflowVector.size();i++){
			if (_cashflowVector[i].getIsValid())
				return i;
		}
		return (int) NaN;
	}

	int cashflowLeg::getCashFlowIndexForAccrualEnd(date accuralEndDate){
		for (unsigned int i=0;i<_cashflowVector.size();i++){
			if (_cashflowVector[i].getAccuralEndDate() == accuralEndDate)
				return i;
		}
		return (int) NaN;
	}

	vector<cashflow> cashflowLeg::getCashFlowVector() {
		return _cashflowVector;
	}

	void cashflowLeg::setCashFlowVector(vector<cashflow> cashflowVector) {
		_cashflowVector=cashflowVector;
	}

	vector<date> cashflowLeg::getAccuralStartDates() {
		vector<date> aDates;

		std::vector<cashflow>::iterator itT=_cashflowVector.begin();
		for (;itT!=_cashflowVector.end();itT++) {
			cashflow aCF=*itT;
			aDates.push_back(aCF.getAccuralStartDate());
		}

		return aDates;
	}

	vector<date> cashflowLeg::getAccuralEndDates() {
		vector<date> aDates;

		std::vector<cashflow>::iterator itT=_cashflowVector.begin();
		for (;itT!=_cashflowVector.end();itT++) {
			cashflow aCF=*itT;
			aDates.push_back(aCF.getAccuralEndDate());
		}

		return aDates;
	}

	void cashflowLeg::markCashFlowValidity(date tradeDate){
		for (unsigned int i=0;i<_cashflowVector.size();i++){
			cashflow cf = _cashflowVector[i];
			if (cf.getPaymentDate()>tradeDate)
				cf.setIsValid(true);
			else
				cf.setIsValid(false);
		}
	}

	void cashflowLeg::printCashFlowLeg() {
		std::vector<cashflow>::iterator itT=_cashflowVector.begin();

		for (;itT!=_cashflowVector.end();itT++) {
			cashflow aCF=*itT;

			aCF.printCashFlow();

		}
	}

	void cashflowLeg::printTimeLine() {
		std::stringstream ss (stringstream::in | stringstream::out);
		ss<<"Accrual Time Line: \n";
		std::vector<cashflow>::iterator itT=_cashflowVector.begin();
		for (;itT!=_cashflowVector.end();itT++) {
			cashflow aCF=*itT;
			ss<<aCF.getAccuralStartDate().toString()<<"\n";
		}
		cout<<ss.str()<<endl;
	}
}