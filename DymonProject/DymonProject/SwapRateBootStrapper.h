//created by Wang Jianwei on 01 Dec 2012
#ifndef SWAPRATEBOOTSTRAPPER_H
#define SWAPRATEBOOTSTRAPPER_H
#include <string>
#include <vector>
#include "AbstractBootStrapper.h"
#include "DiscountCurve.h"
#include "cashflowLeg.h"
#include "Enums.h"
#include "date.h"

using namespace instruments;

namespace utilities {
	class SwapRateBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;
		
		void init(Configuration* cfg);

		SwapRateBootStrapper(point startPoint, date endDate, double swapRate, cashflowLeg* cashflows, DiscountCurve* curve, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo, enums::DayCountEnum dayCount):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_curve = curve;
			_swapRate = swapRate;
			_cashflowVector = cashflows->getCashFlowLeg();
			_dayCount = dayCount;
		}
				
		AbstractInterpolator<date>* bootStrap();
	
		double numericalFunc(double xVal);

	private:

		DiscountCurve* _curve;
		double _swapRate;
		vector<cashflow> _cashflowVector;
		enums::DayCountEnum _dayCount;

		unsigned int _cashflowStartIndex;
		unsigned int _cashflowEndIndex;

		unsigned int findCashFlowIndex(date date0);
	};
}
#endif