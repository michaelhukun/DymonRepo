//created by Wang Jianwei on 01 Dec 2012
#ifndef DEPOSITRATEBOOTSTRAPPER_H
#define DEPOSITRATEBOOTSTRAPPER_H
#include <string>
#include "AbstractBootStrapper.h"
#include "cashflow.h"
#include "Enums.h"
#include "date.h"
#include "Market.h"

using namespace instruments;

namespace utilities {
	class DepositRateBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;

		void init(Configuration* cfg);

		DepositRateBootStrapper(point startPoint, date endDate, cashflow cashFlow, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo, Market market, double bizDaysAfterSpotDF):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_depositRate = cashFlow.getCouponRate();
			_cashFlow = cashFlow;
			_market = market;
			_bizDaysAfterSpotDF = bizDaysAfterSpotDF;
			_dayCountCash = market.getDayCountCashConvention();
		};
				
		AbstractInterpolator<date>* bootStrap();

		double numericalFunc(double x);

	private:

		double _depositRate;
		Market _market;
		cashflow _cashFlow;
		double _bizDaysAfterSpotDF;
		enums::DayCountEnum _dayCountCash;
	};
}
#endif