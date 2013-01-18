//created by Wang Jianwei on 29 Nov 2012
//added functions to get today's year, month and day-Hu Kun 2 Dec

#include "date.h"
#include "dateUtil.h"
#include "fileUtil.h"
#include <string>
#include <sstream>

using namespace std;


namespace utilities {

	date::date() {};
	date::date(unsigned short year, unsigned short month, unsigned short day){
		if (year<=0 || month<=0 || day<=0)
			throw "Year/Month/Day must be larger than 0";
		_year=year;
		_month=month;
		_day=day;
		setJudianDayNumber();
	}

	date::date(std::string dateStr){
		vector<std::string> dayMonthYear = fileUtil::split(dateStr, '/');
		_year=(unsigned short) std::stoul(dayMonthYear[2],NULL,0);
		_month=(unsigned short) std::stoul(dayMonthYear[1],NULL,0);
		_day=(unsigned short) std::stoul(dayMonthYear[0],NULL,0);
		setJudianDayNumber();
	}
	
	date::date(long JDN){
		unsigned short* yearMonthDay = dateUtil::getYearMonthDay(JDN);
		_year = yearMonthDay[0];
		_month = yearMonthDay[1];
		_day = yearMonthDay[2];
		_judianDayNumber = JDN;
	}

	date::~date(){
	}

	void date::setJudianDayNumber(){
		_judianDayNumber = dateUtil::getJudianDayNumber(_year, _month, _day);
	}

	bool date::isEqual(date date0){
		if (_year!=date0.getYear()||_month!=date0.getMonth()||_day!=date0.getDay())
			return false;
		return true;
	}

	long date::getJudianDayNumber(){
		return _judianDayNumber;
	}

	int date::getYear(){
		return _year;
	}

	int date::getMonth(){
		return _month;
	}

	int date::getDay(){
		return _day;
	}

	void date::setDay(unsigned short day){
		_day=day;
		setJudianDayNumber();
	}

	void date::setMonth(unsigned short month){
		_month = month;
		setJudianDayNumber();
	}

	void date::printDate() {
		cout <<_year<<"-"<<_month<<"-"<<_day<<endl;
	}

	string date::toString(){
		std::stringstream ss (stringstream::in | stringstream::out);
		ss<<getDay()<<"/"<<getMonth()<<"/"<<getYear();//"-"<<_judianDayNumber;
		return ss.str();
	}
}