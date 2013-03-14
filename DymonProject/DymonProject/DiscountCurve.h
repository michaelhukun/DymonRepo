//created by Wang Jianwei on 01 Dec 2012
#ifndef DiscountCurve_H
#define DiscountCurve_H
#include "AbstractCurve.h"
#include <vector>
#include "Enums.h"
#include "date.h"

namespace utilities{
	class DiscountCurve: public AbstractCurve<date>{
		
	public:
		
		DiscountCurve():AbstractCurve(){
			_rateType = enums::DF;
		};

		DiscountCurve(std::vector<AbstractInterpolator<date>*>* lineSectionVector):AbstractCurve(lineSectionVector){};

		double getDiscountFactor(date endDate);

		double getDiscountFactor(date beginDate, date endDate);

		double getDiscountFactor(date endDate, double zeroRate);

		double getZeroRate(date endDate);

		double getZeroRate(date endDate, double discountFactor);

		double getZeroRate(date aDate, enums::DayCountEnum dayCount);

		double getZeroRate(date beginDate, date endDate, enums::DayCountEnum dayCount);

		double getZeroRate(date beginDate, date endDate, double discountFactor, enums::DayCountEnum dayCount);

		double getDFChangingZeroRate(date aDate, double zeroRateDiff, enums::DayCountEnum dayCount);

		double getFLiborRate(date forwardStartDate,date forwardEndDate,enums::DayCountEnum dayCount);
	};
}
#endif