//created by Wang Jianwei on 01 Dec 2012
#ifndef DEPOSITRATEBOOTSTRAPPER_H
#define DEPOSITRATEBOOTSTRAPPER_H
#include <string>
#include "AbstractBootStrapper.h"
#include "cashflow.h"
#include "Enums.h"
#include "date.h"
#include "Market.h"
#include "Deposit.h"
#include "DiscountCurve.h"

using namespace instruments;

namespace utilities {
	class DepositRateBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;

		void init(Configuration* cfg);

		DepositRateBootStrapper(point startPoint, date endDate, Deposit* deposit, DiscountCurve* curve, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo, double spotDateDF):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_deposit = deposit;
			_spotDateDF = spotDateDF;
			_curve = curve;
		};
				
		AbstractInterpolator<date>* bootStrap();

		double numericalFunc(double x);

	private:
		
		DiscountCurve* _curve;
		Deposit* _deposit;
		double _spotDateDF;
	};
}
#endif