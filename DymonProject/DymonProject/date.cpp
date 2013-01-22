//created by Wang Jianwei on 29 Nov 2012
//added functions to get today's year, month and day-Hu Kun 2 Dec

#include "date.h"
#include "dateUtil.h"
#include "fileUtil.h"
#include <string>
#include <sstream>
#include "Constants.h"

using namespace std;
using namespace utilities;


namespace utilities {

	date::date() {};
	date::date(unsigned short year, unsigned short month, unsigned short day){
		if (year<=0 || month<=0 || day<=0)
			throw "Year/Month/Day must be larger than 0";
		_year=year;
		_month=month;
		_day=day;
		_isNull=false;
		setJudianDayNumber();
	}

	date::date(std::string dateStr, bool monthBeforeDay){
		if (dateStr==""){
			_isNull = true;
			return;
		}else{
			vector<std::string> dayMonthYear = fileUtil::split(dateStr, '/');
			_year=(unsigned short) std::stoul(dayMonthYear[2],NULL,0);
			_month=(unsigned short) std::stoul(dayMonthYear[monthBeforeDay?0:1],NULL,0);
			_day=(unsigned short) std::stoul(dayMonthYear[monthBeforeDay?1:0],NULL,0);
			_isNull=false;
			setJudianDayNumber();
		}
	}

	date::date(long JDN){
		unsigned short* yearMonthDay = dateUtil::getYearMonthDay(JDN);
		_year = yearMonthDay[0];
		_month = yearMonthDay[1];
		_day = yearMonthDay[2];
		_isNull=false;
		_judianDayNumber = JDN;
	}

	void date::setJudianDayNumber(){
		if (_isNull) return;
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
		if (_isNull) return;
		_day=day;
		setJudianDayNumber();
	}

	void date::setMonth(unsigned short month){
		if (_isNull) return;
		_month = month;
		setJudianDayNumber();
	}

	void date::printDate() {
		if (_isNull){
			cout <<"Date is Null!"<<endl;
		}else{
			cout <<_year<<"-"<<_month<<"-"<<_day<<endl;
		}
	}

	string date::toString(){
		std::stringstream ss (stringstream::in | stringstream::out);
		if (_isNull){
			ss <<"NullDate"<<endl;
		}else{
			ss<<getDay()<<"/"<<getMonth()<<"/"<<getYear();//"-"<<_judianDayNumber;
		}
		return ss.str();
	}
}