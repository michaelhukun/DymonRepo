#include "stdafx.h"
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "RecordHelper.h"
#include "CashFlowLegBuilder.h"
#include <math.h>
#include "Market.h"
#include <vector>
#include "Enums.h"
#include "Swap.h"
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
	//TestBond bondTest;
	//bondTest.runTest();
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

	Market market=Market(enums::USD);

	for(int i=12; i<240; i++){
		date startDate =dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,enums::USD);	
		date tradeDate = dateUtil::getEndDateMonthIncrement(startDate,i);
		//Swap swap1(tradeDate, tenorNumOfMonths, notional, couponRate, yc, market,paymentFreqFixLeg, paymentFreqFloatingLeg, rollAccuralDates);
		//cashflowLeg* fixLeg=swap1.getCashFlowVectorFix();
		//fixLeg->printCashFlowLeg();
		//cashflowLeg* floatLeg=swap1.getCashFlowVectorFloat();
		//cout<<"Swap starting at ["<<tradeDate.toString()<<"] months with par rate ["<<swap1.getParRate(floatLeg,fixLeg,yc)<<"]"<<endl;
		//cout<<swap1.getParRate(floatLeg,fixLeg,yc)<<endl;
		//cout<<SwapPricer(&swap1).getMPV(fixLeg,floatLeg,yc)<<endl;
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

}

void CashFlowLegTest()  {
	
}

void SwapTest() {

}