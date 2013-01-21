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

	cashflowLeg::cashflowLeg(vector<cashflow> cashflowLeg) {
		setCashFlowVector(cashflowLeg);
	}

	cashflow cashflowLeg::getCashFlow(unsigned int index){
		if (index<0||index>=_cashflowLeg.size())
			throw "Index out of range!";

		return _cashflowLeg[index];
	}

	int cashflowLeg::getFirstValidCashFlowIndex(){
		for (unsigned int i=0;i<_cashflowLeg.size();i++){
			if (_cashflowLeg[i].getIsValid())
				return i;
		}
	}

	int cashflowLeg::getCashFlowIndexForAccrualEnd(date accuralEndDate){
		for (unsigned int i=0;i<_cashflowLeg.size();i++){
			if (_cashflowLeg[i].getAccuralEndDate() == accuralEndDate)
				return i;
		}
		return (int) NaN;
	}

	vector<cashflow> cashflowLeg::getCashFlowVector() {
		return _cashflowLeg;
	}

	void cashflowLeg::setCashFlowVector(vector<cashflow> cashflowLeg) {
		_cashflowLeg=cashflowLeg;
	}

	vector<date> cashflowLeg::getAccuralStartDates() {
		vector<date> aDates;

		std::vector<cashflow>::iterator itT=_cashflowLeg.begin();
		for (;itT!=_cashflowLeg.end();itT++) {
			cashflow aCF=*itT;
			aDates.push_back(aCF.getAccuralStartDate());
		}

		return aDates;
	}

	vector<date> cashflowLeg::getAccuralEndDates() {
		vector<date> aDates;

		std::vector<cashflow>::iterator itT=_cashflowLeg.begin();
		for (;itT!=_cashflowLeg.end();itT++) {
			cashflow aCF=*itT;
			aDates.push_back(aCF.getAccuralEndDate());
		}

		return aDates;
	}

	void cashflowLeg::markCashFlowValidity(date tradeDate){
		for (unsigned int i=0;i<_cashflowLeg.size();i++){
			cashflow cf = _cashflowLeg[i];
			if (cf.getPaymentDate()>tradeDate)
				cf.setIsValid(true);
			else
				cf.setIsValid(false);
		}
	}

	void cashflowLeg::printCashFlowLeg() {
		std::vector<cashflow>::iterator itT=_cashflowLeg.begin();

		for (;itT!=_cashflowLeg.end();itT++) {
			cashflow aCF=*itT;

			aCF.printCashFlow();

		}
	}

	void cashflowLeg::printTimeLine() {
		std::stringstream ss (stringstream::in | stringstream::out);
		ss<<"Accrual Time Line: \n";
		std::vector<cashflow>::iterator itT=_cashflowLeg.begin();
		for (;itT!=_cashflowLeg.end();itT++) {
			cashflow aCF=*itT;
			ss<<aCF.getAccuralStartDate().toString()<<"\n";
		}
		cout<<ss.str()<<endl;
	}
}