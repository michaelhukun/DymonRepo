//created by Wang Jianwei on 01 Dec 2012
#ifndef BONDRATEBOOTSTRAPPER_H
#define BONDRATEBOOTSTRAPPER_H
#include <string>
#include "AbstractBootStrapper.h"
#include "cashflow.h"
#include "Enums.h"
#include "date.h"
#include "Market.h"
#include "bond.h"

using namespace instruments;

namespace utilities {
	class BondRateBootStrapper: public AbstractBootStrapper<date>{

	public:

		typedef tuple<date, double> point;

		void init(Configuration* cfg){
			AbstractBootStrapper<date>::init(cfg);
		}

		BondRateBootStrapper(point startPoint, date endDate, Bond* bond, DiscountCurve* curve, enums::interpolAlgo interpolAlgo, 
			enums::NumericAlgo numericAlgo, Market market):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
				_couponRate = bond->getCouponRate();
				_bond = bond;
				_curve = curve;
				_market = market;
				_dayCount = bond->getDayCount();
		};

		AbstractInterpolator<date>* bootStrap();

		double numericalFunc(double x);

	private:

		Bond* _bond;
		DiscountCurve* _curve;
		double _couponRate;
		Market _market;
		enums::DayCountEnum _dayCount;
		AbstractInterpolator<date>* _interpolant;
	};
}
#endif