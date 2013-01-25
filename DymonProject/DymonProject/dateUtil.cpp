//created by Hu Kun 30 Nov 2012
#include "dateUtil.h"
#include "date.h"
#include <math.h>
#include <iostream>
#include "RecordHelper.h"
#include <ctime>

using namespace utilities;
using namespace Session;
using namespace std;

long dateUtil::getJudianDayNumber(unsigned short year, unsigned short month, unsigned short day){
	int	_a=(14-month)/12;
	int _y=year+4800-_a;
	int _m=month+12*_a-3;
	//if (_calendarType==Gregorian){
	return day+(int)((153*_m+2)/5)+365*_y+(int)(_y/4)-(int)(_y/100)+(int)(_y/400)-32045;
	/*}else if (_calendarType==Judian){
		_judianDayNumber=_day+(int)((153*_m+2)/5)+365*_y+(int)(_y/4)-32083;
	}*/
}

long dateUtil::getDaysBetween(date startDate, date endDate){
	return getDaysBetween
		(endDate.getJudianDayNumber(),startDate.getJudianDayNumber());
}

long dateUtil::getDaysBetween(long startJDN, long endJDN){
	return startJDN-endJDN;
}

signed long dateUtil::getBizDaysBetween(date startDate, date endDate){
	return getBizDaysBetween
		(startDate.getJudianDayNumber(),endDate.getJudianDayNumber());
}

signed long dateUtil::getBizDaysBetween(long startJDN, long endJDN){
	
	long numBizDay=0;
	for (long i = 0; i<abs(endJDN-startJDN); i++){
		if (isBizDay(i)) numBizDay++;
	}
	if (startJDN>endJDN) {
		return -numBizDay;
	}
	else {
		return numBizDay;
	}
}

bool dateUtil::isBizDay(date date0){
	return isBizDay(date0.getJudianDayNumber());
}

bool dateUtil::isBizDay(long JDN){
	int dayOfWeek = 1 + (int)fmod(JDN+1,7.0);
	if (dayOfWeek==1||dayOfWeek==7){
		return false;
	}
	return true;
}

bool dateUtil::isHoliday(long JDN, enums::MarketEnum market){
	RecordHelper::HolidayMap holidayMap= RecordHelper::getInstance()->getHolidayMap();
	if (holidayMap.find(market)== holidayMap.end()){
		cout<< "Market not found in Holiday Map: "+market<<endl;
		return false;
	}

	set<long> holidaySet = holidayMap[market];
	if (holidaySet.find(JDN) != holidaySet.end())
		return true;
	return false;
}

bool dateUtil::isHoliday(date aDate, enums::MarketEnum market){
	return isHoliday(aDate.getJudianDayNumber(),market);
}

date dateUtil::getToday(){
	date today(getTodayYear(),getTodayMonth(),getTodayDay());
	return today;
}

int dateUtil::getTodayDay() {
	time_t     rawtime;
	struct tm* timeinfo;

	time( &rawtime );
	timeinfo = localtime( &rawtime );
	return timeinfo->tm_mday;		
}

int dateUtil::getTodayMonth() {
	time_t     rawtime;
	struct tm* timeinfo;

	time( &rawtime );
	timeinfo = localtime( &rawtime );
	return 1+timeinfo->tm_mon;	
}
	
int dateUtil::getTodayYear() {
	time_t     rawtime;
	struct tm* timeinfo;

	time( &rawtime );
	timeinfo = localtime( &rawtime );
	return 1900+timeinfo->tm_year;
}

unsigned short* dateUtil::getYearMonthDay(long JDN){
	long _year, _month, _day;
	JDN = JDN - 1721119 ;
	_year = (4 * JDN - 1) / 146097 ; 
	JDN = 4 * JDN - 1 - 146097 * _year ; 
	_day = JDN / 4 ;
	JDN = (4 * _day + 3) / 1461 ; 
	_day = 4 * _day + 3 - 1461 * JDN ; 
	_day = (_day + 4) / 4 ;
	_month = (5 * _day - 3) / 153 ; 
	_day = 5 * _day - 3 - 153 * _month ; 
	_day = (_day + 5) / 5 ;
	_year = 100 * _year + JDN ;
	if (_month < 10){
		_month = _month + 3;
	}
	else{
		_month = _month - 9 ; 
		_year = _year + 1;
	}
	unsigned short* returnArray = new unsigned short[3]();
	returnArray[0]=(unsigned short) _year;
	returnArray[1]=(unsigned short) _month;
	returnArray[2]=(unsigned short) _day;
	return returnArray;
}

double dateUtil::getAccrualFactor(date startDate,date endDate, enums::DayCountEnum dayCount){
	double accrualFactor;
	int numBizDay=0;
	switch(dayCount){
	case enums::thirty_360US:
		//This day count convention is also called Bond basis.
		if (endDate.getDay()==31 && (startDate.getDay()==30||31))
			endDate.setDay(30);
		if (startDate.getDay()==31)
			startDate.setDay(30);
		accrualFactor = thirty_360(startDate, endDate);
		break;
	case enums::thirthE_360:
		if (startDate.getDay()==31)
			startDate.setDay(30);
		if (endDate.getDay()==31)
			endDate.setDay(30);
		accrualFactor = thirty_360(startDate, endDate);
		break;
	case enums::ACT_360:
		//This day count is also called Money Market basis or Actual 360
		//This is the most used day count convention for money market instruments (maturity below one year).
		accrualFactor = (endDate.getJudianDayNumber()-startDate.getJudianDayNumber())/360.0;
		//cout<<"inside ACT_360"<<endl;
		break;
	case enums::ACT_365:
		//Also called English Money Market basis.
		//The number 365 is used even in a leap year.
		accrualFactor = (endDate.getJudianDayNumber()-startDate.getJudianDayNumber())/365.0;
		//cout<<"inside ACT_365"<<endl;
		break;
	case enums::ACT_ACT:
		if (startDate.getYear()==endDate.getYear()){
			int numDays = endDate.getJudianDayNumber()-startDate.getJudianDayNumber();
			accrualFactor = numDays/(isleapyear(startDate.getYear())?366.0:365.0);
		}else{
			int numStartYearDays = date(startDate.getYear(),12,31).getJudianDayNumber() - startDate.getJudianDayNumber() + 1;
			double startYearFactor = numStartYearDays/(isleapyear(startDate.getYear())?366.0:365.0);
			int numEndYearDays = endDate.getJudianDayNumber() - date(endDate.getYear(),1,1).getJudianDayNumber();
			double endYearFactor = numEndYearDays/(isleapyear(endDate.getYear())?366.0:365.0);
			accrualFactor = startYearFactor + endYearFactor +(endDate.getYear()-startDate.getYear()-1);
		}
		break;
	case enums::BUS_252:
		//Numerator is the number of business days (in a given calendar) from and including the start date up to and excluding the end date.
		numBizDay=0;
		for(long i = startDate.getJudianDayNumber();i<endDate.getJudianDayNumber();i++)
			if (isBizDay(i))
				numBizDay++;
		accrualFactor = numBizDay/252.0;
		break;
	default:
		accrualFactor = (endDate.getJudianDayNumber()-startDate.getJudianDayNumber())/360.0;

	}
	return accrualFactor;
}

double dateUtil::getAccrualFactor(date startDate,date endDate, date refStartDate,date refEndDate, enums::DayCountEnum dayCount){
	double accrualFactor;
	long numDaysAccrual;
	long numDaysRef;
	switch(dayCount){
	case enums::ACT_ACT:
		//To compute the number of days, the period first day is included and the last day is excluded.
		numDaysAccrual = endDate - startDate;
		numDaysRef = refEndDate - refStartDate;
		accrualFactor = numDaysAccrual / (double)numDaysRef;
		break;
	default:
		accrualFactor= getAccrualFactor(startDate, endDate, dayCount);
	}
	return accrualFactor;
}

date dateUtil::dayRollAdjust(date aDate,DayRollEnum aDayRollConvention, enums::MarketEnum market) {
	long adjustedJDN;
	switch(aDayRollConvention){
	case enums::Following:
		adjustedJDN = getFolloingJDN(aDate, market);
		break;
	case enums::Preceding:
		adjustedJDN = getPrecedingJDN(aDate, market);
		break;
	case enums::Mfollowing:
		adjustedJDN = getFolloingJDN(aDate, market);
		if (getYearMonthDay(adjustedJDN)[1]!=aDate.getMonth())
			adjustedJDN = getPrecedingJDN(aDate, market);
		break;
	case enums::Mfollowingbi:	
		adjustedJDN = getFolloingJDN(aDate, market);
		if (getYearMonthDay(adjustedJDN)[1]!=aDate.getMonth()||
			getYearMonthDay(adjustedJDN)[2]>=15)
			adjustedJDN = getPrecedingJDN(aDate, market);
		break;
	case enums::EOM:
		break;
	case enums::Null:
		adjustedJDN = aDate.getJudianDayNumber();
		break;
	}
	date adjustedDate(adjustedJDN);
	return adjustedDate;
}

date dateUtil::getEndDateMonthIncrement(date startDate, int numMonth){
	if (numMonth==0) return startDate;
	short startMonth = startDate.getMonth();
	short endMonth = (startMonth + numMonth)%12;
	int yearIncrement = (startMonth + numMonth)/12;
	yearIncrement = (startMonth + numMonth)<=0?yearIncrement-1:yearIncrement;
	endMonth = endMonth<=0?endMonth+12:endMonth;
	short endYear= startDate.getYear()+yearIncrement;	
	date endDate(endYear, endMonth, startDate.getDay());

	// Adjust the return day to the end of month if the start date is also start of month
	if (startDate.getDay() == getMonthLastDay(startDate.getYear(), startDate.getMonth()))
		endDate.setDay(getMonthLastDay(endDate.getYear(), endDate.getMonth()));
	return endDate;
}

date dateUtil::getEndDate(date startDate, int increment, enums::DayRollEnum dayRoll, enums::MarketEnum market, DateUnit dateUnit){
	date endDate;
	switch(dateUnit){
	case YEAR:
		endDate = date(startDate.getYear()+increment,startDate.getMonth(), startDate.getDay());
		break;
	case MONTH:
		endDate = getEndDateMonthIncrement(startDate, increment);
		break;
	case WEEK:
		endDate = date(startDate.getJudianDayNumber()+increment*7);
		break;
	case DAY:
		endDate = date(startDate.getJudianDayNumber()+increment);
		break;
	case BIZDAY:
		endDate = getBizDateOffSet(startDate,increment, market);
		break;
	}
	endDate = dayRollAdjust(adjustInvalidateDate(endDate,false),dayRoll, market);
	return endDate;
}

date dateUtil::getBizDateOffSet(date refDate, long offset, enums::MarketEnum market) {
	long JDN = refDate.getJudianDayNumber();
	bool forward = offset>=0?true:false;
	for (long i=0; i<abs(offset); i++){
		forward?JDN++:JDN--;
		while(!isBizDay(JDN) || isHoliday(JDN,market)){
			forward?JDN++:JDN--;
		}
	}
	date offsetDate(JDN);
	return offsetDate; 
}
		
int dateUtil::getMonthLastDay(int year, int month){
	unsigned short monthlen[]={31,28,31,30,31,30,31,31,30,31,30,31};
	if (isleapyear(year) && month==2)
		monthlen[1]++;

	int lastDay = monthlen[month-1];
	return lastDay;
}

date dateUtil::adjustInvalidateDate(date aDate, bool forwardAdjust){
	int monthLastDay = getMonthLastDay(aDate.getYear(), aDate.getMonth());
	if (aDate.getDay()>monthLastDay){
		if (forwardAdjust){
			aDate.setMonth(aDate.getMonth()%12+1);
			aDate.setDay(1);
		}	else{
			aDate.setDay(monthLastDay);
		}
	}
	return aDate;
}

bool dateUtil::isleapyear(unsigned short year){
	return (!(year%4) && (year%100) || !(year%400));
}

long dateUtil::getPrecedingJDN(date refDate, enums::MarketEnum market){
	//date adjustedDate = adjustInvalidateDate(refDate,false);
	long JDN = refDate.getJudianDayNumber();
	while(!isBizDay(JDN) || isHoliday(JDN,market)){
		JDN--;
	}
	return JDN;
}

long dateUtil::getFolloingJDN(date refDate, enums::MarketEnum market){
	//date adjustedDate = adjustInvalidateDate(refDate,true);
	long JDN = refDate.getJudianDayNumber();
	while(!isBizDay(JDN) || isHoliday(JDN,market)){
		JDN++;
	}
	return JDN;
}

double dateUtil::thirty_360(date startDate, date endDate){
	int yearFactor = 360*(endDate.getYear()-startDate.getYear());
	int monthFactor = 30*(endDate.getMonth()-startDate.getMonth());
	int dayFactor = endDate.getDay()-startDate.getDay();
	return (yearFactor+monthFactor+dayFactor)/360.0;
}

dateUtil::DateUnit dateUtil::getDateUnit(char letterDateUnit){
	switch(letterDateUnit){
	case 'BD':
		return BIZDAY;
	case 'D':
		return DAY;
	case 'M':
		return MONTH;
	case 'W':
		return WEEK;
	case 'Y':
		return YEAR;
	}
	throw "DateUnit not found: "+letterDateUnit;
}
