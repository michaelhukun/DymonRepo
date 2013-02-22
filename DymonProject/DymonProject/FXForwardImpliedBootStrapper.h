//created by Wang Jianwei on 01 Dec 2012
#ifndef FXFORWARDIMPLIEDBOOTSTRAPPER_H
#define FXFORWARDIMPLIEDBOOTSTRAPPER_H
#include <string>
#include "AbstractBootStrapper.h"
#include "Enums.h"
#include "date.h"
#include "FXForward.h"

using namespace instruments;
	

namespace utilities {
	class FXForwardImpliedBootStrapper: public AbstractBootStrapper<date>{
		
	public:
		
		typedef tuple<date, double> point;

		void init(Configuration* cfg);

		FXForwardImpliedBootStrapper(point startPoint, date endDate, FXForward* forward, enums::interpolAlgo interpolAlgo,
			enums::NumericAlgo numericAlgo):AbstractBootStrapper(startPoint, endDate, interpolAlgo, numericAlgo){
			_forward = forward;
		};
				
		AbstractInterpolator<date>* bootStrap();

		double numericalFunc(double x);

		double getBaseYieldCurveCcyDF(enums::CurrencyEnum baseYieldCurveCcy);

		// Getters and Setters

		void setBaseYieldCurveCcy(enums::CurrencyEnum baseYieldCurveCcy){ _baseYieldCurveCcy = baseYieldCurveCcy; }
		void setForwardImpliedCurveCcy(enums::CurrencyEnum forwardImpliedCurveCcy){ _forwardImpliedCurveCcy = forwardImpliedCurveCcy; }

	private:
		
		FXForward* _forward;
		enums::CurrencyEnum _baseYieldCurveCcy;
		enums::CurrencyEnum _forwardImpliedCurveCcy;
	};
}
#endif