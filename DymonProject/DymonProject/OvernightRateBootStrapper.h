//created by Wang Jianwei on 01 Dec 2012
#ifndef OVERNIGHTRATEBOOTSTRAPPER_H
#define OVERNIGHTRATEBOOTSTRAPPER_H
#include <string>
#include "AbstractBootStrapper.h"
#include "cashflow.h"
#include "Enums.h"
#include "Market.h"
#include "date.h"

using namespace instruments;

namespace utilities {
	class OvernightRateBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;

		void init(Configuration* cfg);

		OvernightRateBootStrapper(point startPoint, date endDate, Deposit* deposit, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_deposit = deposit;
		};
				
		AbstractInterpolator<date>* bootStrap();

		double numericalFunc(double x);

	private:

		Deposit* _deposit;
	};
}
#endif