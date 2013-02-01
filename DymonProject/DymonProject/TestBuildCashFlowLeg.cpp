#include "TestBuildCashFlowLeg.h"
#include "BuilderCashFlowLeg.h"
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
	int paymentFreq = 4;
	int buildDirection =1;
	Market mkt(market);
	enums::DayCountEnum dayCountSwapConvention = mkt.getDayCountSwapConvention();
	enums::DayRollEnum dayRollSwapConvention = mkt.getDayRollSwapConvention();
	enums::DayRollEnum accrualAdjustSwapConvention = mkt.getAccrualAdjustSwapConvention();
	date swapAccrualStartDate = dateUtil::getBizDateOffSet(startDate,mkt.getBusinessDaysAfterSpot(enums::SWAP),market);
	date swapMaturityDate = dateUtil::getEndDate(swapAccrualStartDate,numberOfMonth,dayRollSwapConvention,market,dateUtil::MONTH);
	BuilderCashFlowLeg builderFromTenor(enums::SWAP,startDate,numberOfMonth,1,1,paymentFreq, market);
	BuilderCashFlowLeg builderFromMaturity(enums::SWAP, startDate, swapMaturityDate,numberOfMonth, 1,1,paymentFreq,market,buildDirection);
	cashflowLeg* cashflowLegFromMaturity = builderFromMaturity.getCashFlowLeg();
	cashflowLeg* cashflowLegFromTenor = builderFromTenor.getCashFlowLeg();
	cout<<"Cash flow building Test, start date ["<<startDate.toString()<<"], number of month ["<<numberOfMonth<<"]"<<endl;
	bool result = compareResult(cashflowLegFromMaturity,cashflowLegFromTenor);
	if (result==true) cout<<"Test Passed!"<<endl;
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