//created by Wang Jianwei on 01 Dec 2012
#ifndef SWAPRATEBOOTSTRAPPER_H
#define SWAPRATEBOOTSTRAPPER_H
#include <string>
#include <vector>
#include "AbstractBootStrapper.h"
#include "DiscountCurve.h"
#include "cashflowLeg.h"
#include "cashflow.h"
#include "Enums.h"
#include "Swap.h"
#include "date.h"

using namespace instruments;
using namespace std;

namespace utilities {
	class SwapRateBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;
		
		void init(Configuration* cfg);

		SwapRateBootStrapper(point startPoint, date endDate, Swap* swap, DiscountCurve* curve, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_curve = curve;
			_swap = swap;
			_cashflowVector = _swap->getCashFlowLegFix()->getCashFlowVector();
		}
				
		AbstractInterpolator<date>* bootStrap();
	
		double numericalFunc(double xVal);

	private:

		DiscountCurve* _curve;
		Swap* _swap;

		std::vector<cashflow> _cashflowVector;
		int _cashflowStartIndex;
		int _cashflowEndIndex;
		int findCashFlowIndex(date date0);
	};
}
#endif