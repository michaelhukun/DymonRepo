//created by Wang Jianwei on 01 Dec 2012
#ifndef EURODOLLARFUTUREBOOTSTRAPPER_H
#define EURODOLLARFUTUREBOOTSTRAPPER_H
#include <string>
#include "AbstractBootStrapper.h"
#include "cashflow.h"
#include "Enums.h"
#include "date.h"
#include "Market.h"
#include "EuroDollarFuture.h"
#include "DiscountCurve.h"

using namespace instruments;

namespace utilities {
	class EuroDollarFutureBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;

		void init(Configuration* cfg);

		EuroDollarFutureBootStrapper(point startPoint, date endDate, EuroDollarFuture* future, DiscountCurve* curve, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_future = future;
			_curve = curve;
		};
				
		AbstractInterpolator<date>* bootStrap();

		double numericalFunc(double x);

	private:
		
		DiscountCurve* _curve;
		EuroDollarFuture* _future;
	};
}
#endif