//created by Wang Jianwei on 01 Dec 2012
#ifndef BondCurveBuilder_H
#define BondCurveBuilder_H
#include "AbstractCurve.h"
#include "DiscountCurve.h"
#include "AbstractBuilder.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "DiscountCurve.h"
#include "bond.h"
#include <vector>

using namespace instruments;
typedef tuple<date, double> point;

namespace utilities{
	class BondCurveBuilder: public AbstractBuilder{
		
	public:
		
		BondCurveBuilder(enums::CurrencyEnum ccyEnum):AbstractBuilder(){_market = Market(ccyEnum);}

		void init(Configuration* cfg);

		DiscountCurve* build(Configuration* cfg);

		void buildSection(DiscountCurve* yc);

		Market getMarket(){return _market;}
		void setMarket(Market market){_market = market;}
		
		int getCouponFreqency(){return _couponFreqency;}
		void seCouponFreqency(int couponFreqency){_couponFreqency=couponFreqency;}

		int getTimeLineBuildDirectiony(){return _timeLineBuildDirection;}
		void setTimeLineBuildDirection(int timeLineBuildDirection){_timeLineBuildDirection=timeLineBuildDirection;}

		bool getRollAccuralDates(){return _rollAccuralDates;}
		void setRollAccuralDates(bool rollAccuralDates){_rollAccuralDates = rollAccuralDates;}

	private:

		Market _market;
		int _couponFreqency;
		int _timeLineBuildDirection;
		bool _rollAccuralDates;
		point _curvePointer;
		date _curveStartDate;
		enums::DayCountEnum _bondCurveDayCount;
		RateType _interpolRateType;
		std::map<long, Bond> _bondMap;
	};
}
#endif