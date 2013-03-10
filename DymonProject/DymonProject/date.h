//created by Hu Kun on 29 Nov 2012
//added functions to get today's year, month and day-Hu Kun 2 Dec

#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <iostream>
#include <string>

using namespace std;

namespace utilities {
	class date {

	public:
		
		date(){};
		date(unsigned short year, unsigned short month, unsigned short day);
		date(std::string dateStr, bool monthBeforeDay);
		date(long JDN);
		~date(){};

		// Getters and Setters
		int getYear(){	return _year; }
		int getMonth(){ return _month; }
		int getDay(){ return _day; }
		long getJudianDayNumber() const{ return _judianDayNumber; }
		bool isNull(){ return _isNull; }

		void setDay(unsigned short day);
		void setMonth(unsigned short month);
		void setJudianDayNumber(long JDN);

		// Methods
		bool isEqual(date date0);
		void printDate();
		std::string toString();
		bool isMonthEnd();
				
		int compare (const date d) {
			if (_judianDayNumber<d.getJudianDayNumber())
				return -1;
			else if (_judianDayNumber>d.getJudianDayNumber())
				return 1;
			else
				return 0;
		}
		
		bool operator != (const date d) {	return compare(d)!=0;}

		bool operator == (const date d) {	return !compare(d);}

		bool operator < (const date d) { return compare(d)<0; }
		
		bool operator > (const date d) { return compare(d)>0; }

		bool operator <= (const date d) { return compare(d)<=0; }
		
		bool operator >=(const date d) { return compare(d)>=0; }

		long operator - (const date d) { return _judianDayNumber - d.getJudianDayNumber(); }
		
		date operator + (int dayIncrement){
			date incrementedDate(_judianDayNumber+dayIncrement);
			return incrementedDate;
		}

	private:

		int _year;
		int _month;
		int _day;
		bool _isNull;
		long _judianDayNumber;
		void setJudianDayNumber();
		
	};
}
#endif