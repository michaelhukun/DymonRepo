#include "TestBuildCashFlowLeg.h"
#include "CashFlowLegBuilder.h"
#include "dateUtil.h"
#include "cashflow.h"

using namespace UnitTest;

void TestBuildCashFlowLeg::runTest(){
	_EPSILON = 0.000001;
	BuildCashFlowLegTestSuit();
}

void TestBuildCashFlowLeg::BuildCashFlowLegTestSuit(){
	buildCashFlowLegTest(dateUtil::getToday(),60,_EPSILON,USD);
	buildCashFlowLegTest(dateUtil::getToday(),500,_EPSILON,USD);
}

void TestBuildCashFlowLeg::buildCashFlowLegTest(date startDate, int numberOfMonth, double tolerance, enums::CurrencyEnum market){

}

bool TestBuildCashFlowLeg::compareResult(cashflowLeg* cashflowLegFromMaturity, cashflowLeg* cashflowLegFromTenor){
	vector<cashflow> cashflowsMaturity = cashflowLegFromMaturity->getCashFlowVector();
	vector<cashflow> cashflowsTenor = cashflowLegFromTenor->getCashFlowVector();
	for (unsigned int i =0; i<cashflowsMaturity.size();i++){
		cashflow maturityCF = cashflowsMaturity[i];
		cashflow tenorCF = cashflowsTenor[i];
		if (!maturityCF.isDateEqual(tenorCF)){
			cout<<"Cashflow derived from maturity is not the same as cashflow derived from tenor"<<endl;
			cout<<"Cashflow from Maturity: ";
			maturityCF.printCashFlow();
			cout<<"Cashflow from Tenor: ";
			tenorCF.printCashFlow();
			cout<<"\n";
			return false;
		}
	}
	return true;
}