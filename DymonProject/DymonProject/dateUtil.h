//created by Wang Jianwei on 30 Nov 2012

#ifndef _DATEUTIL_H
#define _DATEUTIL_H

#include "date.h"
#include "Enums.h"

using namespace enums;

namespace utilities {
	class dateUtil {

	public:
		enum DateUnit{BIZDAY,DAY,WEEK,MONTH,YEAR,ON, SN, TN};

		static long getJudianDayNumber(unsigned short year, unsigned short month, unsigned short day);

		static signed long getDaysBetween(date startDate, date endDate);

		static signed long getBizDaysBetween(date startDate, date endDate);

	    static bool isBizDay(date date0);

		static bool isHoliday(date aDate, enums::CurrencyEnum market);
		
		static date getToday();
		static int getTodayYear();
		static int getTodayMonth();
		static int getTodayDay();
		static unsigned short* getYearMonthDay(long JDN);
		static int getMonthLastDay(int year, int month);

		//to be used by instruments namespaces to calc dates
		static date getBizDateOffSet(date refDate, long offset, enums::CurrencyEnum market);
		static date getEndDate(date refDate, int increment, enums::DayRollEnum dayRoll, enums::CurrencyEnum market, DateUnit dateUnit);
		static date dayRollAdjust(date aDate,enums::DayRollEnum aDayRollConvention, enums::CurrencyEnum market);
		static double getAccrualFactor(date startDate,date endDate, enums::DayCountEnum dayCount);
		static double getAccrualFactor(date startDate,date endDate, date refStartDate,date refEndDate, enums::DayCountEnum dayCount);
		static DateUnit getDateUnit(std::string letterDateUnit);
		static date getEndDateMonthIncrement(date refDate, int numMonth);

	private:

		static long getDaysBetween(long startJDN, long endJDN);		
		static long getBizDaysBetween(long startJDN, long endJDN);
	    static bool isBizDay(long JDN);
		static bool isHoliday(long JDN, enums::CurrencyEnum market);
		static bool isleapyear(unsigned short year);
		static long getPrecedingJDN(date refDate, enums::CurrencyEnum market);
		static long getFolloingJDN(date refDate, enums::CurrencyEnum market);
		static date adjustInvalidateDate(date aDate, bool forwardAdjust);
		static double thirty_360(date startDate, date endDate);
	};
}

#endif