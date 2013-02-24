//created by Wang Jianwei on 01 Dec 2012
//Added cashflowleg creating - Kun
#ifndef SwapCurveBuilder_H
#define SwapCurveBuilder_H
#include "AbstractCurve.h"
#include "DiscountCurve.h"
#include "AbstractBuilder.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include <vector>

using namespace instruments;
typedef tuple<date, double> point;

namespace utilities{
	class SwapCurveBuilder: public AbstractBuilder{
		
	public:
		
		SwapCurveBuilder(){}
		~SwapCurveBuilder(){}
		SwapCurveBuilder(enums::CurrencyEnum ccyEnum):AbstractBuilder(){_market = Market(ccyEnum);}

		// Methods
		void init(Configuration* cfg);
		DiscountCurve* build(Configuration* cfg);
		void buildDepositSection(DiscountCurve* yc);
		void buildSwapSection(DiscountCurve* yc);

		// Getters and Setters
		Market getMarket(){return _market;}
		void setMarket(Market market){_market = market;}

		int getFloatFrequency(){return _floatFreqency;}
		void setFloatFrequency(int floatFreqency){_floatFreqency=floatFreqency;}

		int getFixFreqency(){return _fixFreqency;}
		void setFixFreqency(int fixFreqency){_fixFreqency=fixFreqency;}

		int getTimeLineBuildDirectiony(){return _timeLineBuildDirection;}
		void setTimeLineBuildDirection(int timeLineBuildDirection){_timeLineBuildDirection=timeLineBuildDirection;}

		int getBizDaysAfterSpot(){return _bizDaysAfterSpot;}
		void setBizDaysAfterSpot(int bizDaysAfterSpot){_bizDaysAfterSpot=bizDaysAfterSpot;}

		bool getRollAccuralDates(){return _rollAccuralDates;}
		void setRollAccuralDates(bool rollAccuralDates){_rollAccuralDates = rollAccuralDates;}

		double getBizDaysAfterSpotDF(){return _spotDateDF;}
		void setBizDaysAfterSpotDF(double spotDateDF){_spotDateDF = spotDateDF;}

	private:

		Market _market;
		int _floatFreqency;
		int _fixFreqency;
		int _timeLineBuildDirection;
		bool _rollAccuralDates;
		int _bizDaysAfterSpot;
		double _spotDateDF;
		date _spotDate;
		point _curvePointer;
		date _curveStartDate;
	};
}
#endif