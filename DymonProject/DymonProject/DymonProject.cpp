#include "stdafx.h"
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "RecordHelper.h"
#include "BuilderCashFlowLeg.h"
#include <math.h>
#include "Market.h"
#include <vector>
#include "Enums.h"
#include "swap.h"
#include "TestNumerical.h"
#include "TestInterpolator.h"
#include "TestDateUtil.h"
#include "SwapCurveBuilder.h"
#include "DiscountCurve.h"
#include "LinearInterpolator.h"
#include "TestBuildCashFlowLeg.h"
#include "TestOption.h"
#include "TestDiscountCurve.h"
#include "TestBond.h"
#include "AbstractSurface.h"
#include "TestSurface.h"
#include "swaption.h"
#include "SwaptionVolFileSource.h"
#include "SwaptionVolCube.h"
#include "SwaptionVolCubeBuilder.h"
#include "marketdata.h"

using namespace utilities;
using namespace std;
using namespace instruments;
using namespace Session;
using namespace UnitTest;
using namespace enums;
using namespace Markets;

void LoadInitialData();
void DateUtilTest();
void CashFlowLegTest();
void CashFlowTest();
void SwapTest();
void SwaptionTest();
void unitTest();
void forwardStartingSwap(DiscountCurve* yc);

int _tmain(int argc, _TCHAR* argv[])
{
	LoadInitialData();
	MarketData::getInstance()->buildAll();
	unitTest();

	//DAO::SwaptionATMVolMatrixFileSource aSwaptionTest;
	//aSwaptionTest.swaptionTest();
	//DiscountCurve* yc = buildDiscountCurve();
	//forwardStartingSwap(yc);
	//SwaptionTest();
}		

void unitTest(){	
	//TestDateUtil dateUtilTest;
	//dateUtilTest.runTest();	
	//CashFlowTest();
    //CashFlowLegTest();
	//TestNumerical numericalTest;
	//numericalTest.runTest();
	//TestInterpolator interpolatorTest;
	//interpolatorTest.runTest();
	//SwapTest();
	//TestBuildCashFlowLeg buildCashFlowLegTest;
	//buildCashFlowLegTest.runTest();
	//TestOption optionTest;
	//optionTest.runTest();
	//TestDiscountCurve discountCurveTest;
	//discountCurveTest.runTest();
	//TestSurface surfaceTest;
	//surfaceTest.runTest();
	TestBond bondTest;
	bondTest.runTest();
}

void SwaptionTest(){
	SwaptionVolCube* svc = MarketData::getInstance()->getSwaptionVolCube(USD);
	cout<<svc->getVol(0,12,12)<<endl;
	cout<<svc->getVol(0,120,12)<<endl;
	cout<<svc->getVol(0,120,24)<<endl;
	cout<<svc->getVol(0,120,36)<<endl;
	cout<<svc->getVol(0,18,36)<<endl;
	cout<<svc->getVol(10,12,36)<<endl;

	Market swaptionMarket=Market(enums::USD);

	for(int i=-200; i<=200; i=i+5){
		Swaption swt1(swaptionMarket, enums::Payer,12,i,24);
		cout<<"Swaption Premium ATM"<<i<<": "<<swt1.getMPV()<<" vol: "<<swt1.getVol()<<endl;
	}
}

void forwardStartingSwap(DiscountCurve* yc){
	int tenorNumOfMonths = 24;	
	double notional=1000000;
	double couponRate=0.03;
	int paymentFreqFixLeg=2;
	int paymentFreqFloatingLeg=4;
    bool rollAccuralDates=true;

	Market fixLegCurr=Market(enums::USD);
	Market floatingLegCurr=Market(enums::USD);

	for(int i=12; i<240; i++){
		date startDate =dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,enums::USD);	
		date tradeDate = dateUtil::getEndDateMonthIncrement(startDate,i);
		Swap swap1(tradeDate, tenorNumOfMonths, notional, couponRate, yc, fixLegCurr, floatingLegCurr,paymentFreqFixLeg, paymentFreqFloatingLeg, rollAccuralDates);
		cashflowLeg* fixLeg=swap1.getCashFlowVectorFix();
		//fixLeg->printCashFlowLeg();
		cashflowLeg* floatLeg=swap1.getCashFlowVectorFloat();
		//cout<<"Swap starting at ["<<tradeDate.toString()<<"] months with par rate ["<<swap1.getParRate(floatLeg,fixLeg,yc)<<"]"<<endl;
		//cout<<swap1.getParRate(floatLeg,fixLeg,yc)<<endl;
		cout<<swap1.getMPV(fixLeg,floatLeg,yc)<<endl;
	}
}

void buildSampleCurve(){

	typedef tuple<date, double> point;
	date date0(10005);
	date date1(10006);
	date date2(10007);
	point point1(date0, 1);
	point point2(date1, 2);
	point point3(date2, 2.5);
	DiscountCurve* yc = new DiscountCurve();
	LinearInterpolator<date>* li1 = new LinearInterpolator<date>(point1, point2);
	LinearInterpolator<date>* li2 = new LinearInterpolator<date>(point2, point3);
	yc->insertLineSection(li1);
	yc->insertLineSection(li2);
}

void LoadInitialData(){
	cout << "******** RecordHelper Test ********" << endl;
	RecordHelper* recordHelper = RecordHelper::getInstance();
	recordHelper->init(Configuration::getInstance());
}

void CashFlowTest() {

	cout << "******** CashFlow Test starts********" << endl;
	date fixingDate(2013,11,2);
	date paymentDate(2014,2,6);
	date accrualStartDate(2013,11,3);
	date accrualEndDate(2014,2,5);
	double notional=1000000;
	double couponRate=0.05;
	int paymentFreq=4;
	Market cashFlowCurr=Market(enums::USD);
	cashFlowCurr.setDayCountCashConvention(enums::ACT_360);
	cashFlowCurr.setDayCountSwapConvention(enums::ACT_ACT);
	cashFlowCurr.setDayRollCashConvention(enums::Mfollowing);

	cashflow testCashFlow(couponRate,notional,fixingDate, paymentDate,accrualStartDate, accrualEndDate, cashFlowCurr, true);
	testCashFlow.printCashFlow();
	cout << "******** CashFlow Test ends********" << endl;
}

void CashFlowLegTest()  {
	date startDate(2012,12,21);
	date maturityDate(2014,12,25);
	//date accuralStartDate(2013,11,3);
	//date accuralEndDate(2014,2,5);
	
	double notional=1000000;
	double couponRate=0.05;
	int paymentFreq=4;
	//build from start to end (build forward)
	int buildDirection=1;
	bool rollAccuralDates=false;

	Market cashFlowLegCurr=Market(enums::USD);
	cashFlowLegCurr.setDayCountCashConvention(enums::ACT_360);
	cashFlowLegCurr.setDayCountSwapConvention(enums::ACT_ACT);
	cashFlowLegCurr.setDayRollCashConvention(enums::Mfollowing);

	BuilderCashFlowLeg testCashFlowLeg(enums::SWAP, startDate, maturityDate, 2, couponRate,notional, paymentFreq, enums::USD, buildDirection);

	cout << "******** CashFlowLeg Build Test starts********" << endl;

	std::vector<cashflow> cfVector=testCashFlowLeg.getCashFlowLeg()->getCashFlowVector();
	std::vector<cashflow>::iterator it=cfVector.begin();

	cout<<"start date="<<startDate.toString()<<endl;
	cout<<"maturity date="<<maturityDate.toString()<<endl<<endl;
	
	int i=0;
	for (;it!=cfVector.end();it++) {
	 cashflow aCF(*it);
	 cout<<"***CF stream #"<<++i<<":"<<endl;
	 aCF.printCashFlow();	 
	}

	cout <<"Total number of CF streams="<<i<<endl;
	std::cout << (-20 % 12) << std::endl;
	cout<<"****************************************************"<<endl<<endl;

	buildDirection=-1;
	BuilderCashFlowLeg testCashFlowLegReverse(enums::SWAP,startDate, maturityDate, 2, couponRate,notional, paymentFreq, enums::USD, buildDirection);

	
	cout << "******** CashFlowLeg ReverseBuild Test starts********" << endl;

	std::vector<cashflow> cfVectorR=testCashFlowLeg.getCashFlowLeg()->getCashFlowVector();
	std::vector<cashflow>::iterator itR=cfVectorR.begin();

	cout<<"start date="<<startDate.toString()<<endl;
	cout<<"maturity date="<<maturityDate.toString()<<endl<<endl;

	i=0;
	for (;itR!=cfVectorR.end();itR++) {
	 cashflow aCF(*itR);
	 cout<<"***CF stream #"<<++i<<":"<<endl;
	 aCF.printCashFlow();	 
	}

	cout <<"Total number of CF streams="<<i<<endl<<endl;

	int tenorNumMonth=12;

	BuilderCashFlowLeg testCashFlowLegTenor(enums::SWAP,startDate, tenorNumMonth, couponRate, notional,  paymentFreq, enums::USD);

	cout << "******** CashFlowLeg TenorBuild Test starts********" << endl;

	std::vector<cashflow> cfVectorT=testCashFlowLegTenor.getCashFlowLeg()->getCashFlowVector();
	std::vector<cashflow>::iterator itT=cfVectorT.begin();
	
	cout<<"start date="<<startDate.toString()<<endl;
	cout<<"tenorNumOfMonths="<<tenorNumMonth<<endl<<endl<<endl;

	i=0;
	for (;itT!=cfVectorT.end();itT++) {
	 cashflow aCF(*itT);
	 cout<<"***CF stream #"<<++i<<":"<<endl;
	 aCF.printCashFlow(); 
	}

	cout <<"Total number of CF streams="<<i<<endl<<endl;

	string s;
	getline(cin,s);
}

void SwapTest() {
	date tradeDate(2012,12,21);
	date maturityDate(2062,12,25);
	
	
	double notional=1000000;
	double couponRate=0.05;
	int paymentFreqFixLeg=2;
	int paymentFreqFloatingLeg=4;
	//build from start to end (build forward)
	int buildDirection=1;
	bool rollAccuralDates=false;
	int tenorInMonth = 50;

	typedef tuple<date, double> point;

	SwapCurveBuilder* builder = new SwapCurveBuilder(USD);
	builder->init(Configuration::getInstance());
	DiscountCurve* yc = builder->build(NULL);

	Market fixLegCurr=Market(enums::USD);
	Market floatingLegCurr=Market(enums::USD);
	
	fixLegCurr.setDayCountCashConvention(enums::ACT_360);
	fixLegCurr.setDayCountSwapConvention(enums::ACT_ACT);
	fixLegCurr.setDayRollCashConvention(enums::Mfollowing);
	
	floatingLegCurr.setDayCountCashConvention(enums::ACT_360);
	floatingLegCurr.setDayCountSwapConvention(enums::ACT_ACT);
	floatingLegCurr.setDayRollCashConvention(enums::Mfollowing);

	Swap swap1(tradeDate, maturityDate, notional, tenorInMonth, couponRate, yc, fixLegCurr, floatingLegCurr,paymentFreqFixLeg, paymentFreqFloatingLeg, rollAccuralDates,buildDirection);

	cout<<"tradeDate=";
	tradeDate.printDate();
	cout<<endl;

	cout<<"maturityDate=";
	maturityDate.printDate();
	cout<<endl;
	
	cashflowLeg* fixLeg=swap1.getCashFlowVectorFix();
	cashflowLeg* floatLeg=swap1.getCashFlowVectorFloat();
	
	cout<<"MPV="<<swap1.getMPV(fixLeg,floatLeg,yc)<<endl;
	
	cout<<"ParRate="<<swap1.getParRate(fixLeg,floatLeg,yc)<<endl;
	
	cout<<"========================Fix Leg of Swap1 is:========================="<<endl;
	//swap1.printCashflowLegFix();


	cout<<"========================Floating Leg of Swap1 is:===================="<<endl;
	//swap1.printCashflowLegFloat();




}