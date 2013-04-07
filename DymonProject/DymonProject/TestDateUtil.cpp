//created by Wang Jianwei on 1 Dec 2012
#include "TestDateUtil.h"
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>

using namespace UnitTest;
using namespace utilities;
using namespace std;

typedef AbstractTest super;

void TestDateUtil::init(Configuration* cfg){
	_isEnabled = cfg->getProperty("UnitTest.DateUtil",true,"")=="true"?true:false;
	_EPSILON = 0.0000000001;
}

void TestDateUtil::runTest(){
	if(!_isEnabled) return;
	bizDateAdjustSuit();
	dayRollAdjustTestSuit();
	getEndDateTestSuit();
	DayCountTestSuit();

}

void TestDateUtil::bizDateAdjustSuit(){
	cout << endl<< "******** BizDateAdjust Test ********" << endl;
	{
		date date0(2012,12,19);
		date date1 = dateUtil::getBizDateOffSet(date0, 4, USD);
		date date2(2012,12,25);
		compareResult("BizDateAdjust-1", date1,date2);}
	{
		date date0(2012,12,19);
		date date1 = dateUtil::getBizDateOffSet(date0, -4, USD);
		date date2(2012,12,13);
		compareResult("BizDateAdjust-2", date1,date2);}
}

void TestDateUtil::dayRollAdjustTestSuit(){
	cout << "******** DayRollAdjust Test ********" << endl;
	{
		date date0(2456270);
		date date1(2012,12,8);
		compareResult("DayRollAdjust-1", date0,date1);}
	{
		date date0(2011,9,18);
		date date1 = dateUtil::dayRollAdjust(date0, Following, USD);
		date date2(2011,9,19);
		compareResult("DayRollAdjust-2", date1,date2);}
	{
		date date0(2011,9,18);
		date date1 = dateUtil::dayRollAdjust(date0, Preceding, USD);
		date date2(2011,9,16);
		compareResult("DayRollAdjust-3", date1,date2);}
	{
		date date0(2011,7,30);
		date date1 = dateUtil::dayRollAdjust(date0, Mfollowing, USD);
		date date2(2011,7,29);		
		compareResult("DayRollAdjust-4", date1,date2);}
	{
		date date0(2011,7,30);
		date date1 = dateUtil::dayRollAdjust(date0, Mfollowingbi, USD);
		date date2(2011,7,29);		
		compareResult("DayRollAdjust-5", date1,date2);}
	{
		date date0(2011,10,15);
		date date1 = dateUtil::dayRollAdjust(date0, Mfollowingbi, USD);
		date date2(2011,10,14);		
		compareResult("DayRollAdjust-6", date1,date2);}
}

void TestDateUtil::getEndDateTestSuit(){
	cout << "******** GetEndDate Test ********" << endl;
	{
		date date0(2012,1,31);
		date date1 = dateUtil::getEndDate(date0, 1, Preceding,USD,dateUtil::MONTH);
		date date2(2012,2,29);
		compareResult("GetEndDate-1", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, 4, Preceding,USD,dateUtil::MONTH);
		date date2(2013,2,28);
		compareResult("GetEndDate-2", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, 4, Following,USD,dateUtil::MONTH);
		date date2(2013,2,28);
		compareResult("GetEndDate-3", date1,date2);}
	{
		date date0(2012,3,31);
		date date1 = dateUtil::getEndDate(date0, 9, Following,USD,dateUtil::MONTH);
		date date2(2012,12,31);
		compareResult("GetEndDate-4", date1,date2);}
	{
		date date0(2013,3,19);
		date date1 = dateUtil::getEndDate(date0, 21, Following,USD,dateUtil::MONTH);
		date date2(2014,12,19);
		compareResult("GetEndDate-5", date1,date2);}
	{
		date date0(2013,3,19);
		date date1 = dateUtil::getEndDate(date0, 22, Following,USD,dateUtil::MONTH);
		date date2(2015,1,19);
		compareResult("GetEndDate-6", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, 4, Mfollowing,USD,dateUtil::MONTH);
		date date2(2013,2,28);
		compareResult("GetEndDate-7", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, -1, Mfollowing,USD,dateUtil::MONTH);
		date date2(2012,9,28);
		compareResult("GetEndDate-8", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, -10, Mfollowing,USD,dateUtil::MONTH);
		date date2(2011,12,30);
		compareResult("GetEndDate-9", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, -11, Mfollowing,USD,dateUtil::MONTH);
		date date2(2011,11,30);
		compareResult("GetEndDate-10", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, -12, Mfollowing,USD,dateUtil::MONTH);
		date date2(2011,10,31);
		compareResult("GetEndDate-11", date1,date2);}
	{
		date date0(2012,10,31);
		date date1 = dateUtil::getEndDate(date0, -24, Mfollowing,USD,dateUtil::MONTH);
		date date2(2010,10,29);
		compareResult("GetEndDate-12", date1,date2);}
}

void TestDateUtil::DayCountTestSuit(){
	cout << "******** DayCount Test ********" << endl;
	{
		date sd(2012,10,31);
		date ed(2012,11,30);
		cout<<(dateUtil::getAccrualFactor(sd,ed,thirty_360US)==30/360.0)<<endl;}
	{
		date sd(2012,11,30);
		date ed(2012,12,31);
		cout<<(dateUtil::getAccrualFactor(sd,ed,thirty_360US)==30/360.0)<<endl;}
	{
		date sd(2012,7,31);
		date ed(2012,8,31);
		cout<<(dateUtil::getAccrualFactor(sd,ed,thirthE_360)==30/360.0)<<endl;}
	{
		date sd(2011,3,1);
		date ed(2011,7,3);
		date rsd(2011,3,1);
		date red(2011,9,1);
		cout<<(dateUtil::getAccrualFactor(sd, ed, rsd, red, ACT_ACT)-124/184.0<pow(10.0,-12))<<endl;}
	{
		date sd(2002,8,15);
		date ed(2002,11,4);
		date rsd(2002,8,15);
		date red(2003,2,15);
		cout<<(dateUtil::getAccrualFactor(sd, ed, rsd, red, ACT_ACT)-83/184.0<pow(10.0,-12))<<endl;}
	//{
	//	date sd(2011,12,30);
	//	date ed(2013,1,2);
	//	cout<<(dateUtil::getAccrualFactor(sd,ed,ACT_ACT)-(367/365.0+1+1/365.0)<pow(10.0,-12))<<endl;}
	{
		date sd(2012,12,7);
		date ed(2012,12,17);
		cout<<(dateUtil::getAccrualFactor(sd,ed,BUS_252)-(6/252.0)<pow(10.0,-12))<<endl;}
}

void TestDateUtil::compareResult(std::string testName, date derivedVal, date expectedVal){
	std::stringstream ss (stringstream::in | stringstream::out);
	if (derivedVal.isEqual(expectedVal)){
		ss<<testName<<" Test Passed: derived date ["<<derivedVal.toString()<<"]";
	}else{
		ss<<testName<<" Test Failed: derived date ["<<derivedVal.toString()<<"], expected date ["+expectedVal.toString()+"]";
	}
	cout<<ss.str()<<endl;
}