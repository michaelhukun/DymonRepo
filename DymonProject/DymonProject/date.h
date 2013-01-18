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
		
		date();
		date(unsigned short year, unsigned short month, unsigned short day);
		date::date(std::string dateStr);
		date(long JDN);
		~date();
		int getYear();
		int getMonth();
		int getDay();
		void setDay(unsigned short day);
		void setMonth(unsigned short month);
		void setJudianDayNumber(long JDN);
		long getJudianDayNumber();
		bool isEqual(date date0);
		void printDate();
		std::string toString();
				
		int compare (date d) {
			if (_judianDayNumber<d.getJudianDayNumber())
				return -1;
			else if (_judianDayNumber>d.getJudianDayNumber())
				return 1;
			else
				return 0;
		}
		
		bool operator != (date d) {	return compare(d)!=0;}

		bool operator == (date d) {	return !compare(d);}

		bool operator < (date d) { return compare(d)<0; }
		
		bool operator > (date d) { return compare(d)>0; }

		bool operator <= (date d) { return compare(d)<=0; }
		
		bool operator >=(date d) { return compare(d)>=0; }

		long operator - (date d) { return _judianDayNumber - d.getJudianDayNumber(); }
		
		date operator + (int dayIncrement){
			date incrementedDate(_judianDayNumber+dayIncrement);
			return incrementedDate;
		}

	private:

		int _year;
		int _month;
		int _day;
		long _judianDayNumber;
		void setJudianDayNumber();
		
	};
}
#endif