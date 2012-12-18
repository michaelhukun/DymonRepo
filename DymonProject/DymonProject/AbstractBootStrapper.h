//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTBOOTSTRAPPER_H
#define ABSTRACTBOOTSTRAPPER_H
#include "AbstractSession.h"
#include "AbstractCurve.h"
#include "AbstractInterpolator.h"
#include "AbstractNumerical.h"
#include "Enums.h"
#include <vector>
#include "AbstractSession.h"

using namespace Session;
using namespace enums;

namespace utilities{
	class AbstractBootStrapper: public AbstractSession{
	public:
		typedef tuple<date, double> point;

		virtual void init(Configuration* cfg){};

		AbstractBootStrapper(point startPoint, date endDate,enums::interpolAlgo interpolAlgo,
		enums::NumericAlgo numericAlgo){
			_startPoint = startPoint;
			_endDate = endDate;
			_interpolAlgo = interpolAlgo;
			_numericAlgo = numericAlgo;
		}

		virtual AbstractInterpolator* bootStrap(){return NULL;};

		virtual double numericalFunc(double x){return 0;};

	protected:
	
		point _startPoint;

		date _endDate;

		enums::interpolAlgo _interpolAlgo;

		enums::NumericAlgo _numericAlgo; 

		double _tolerance;

		int _iterateCount;

		int _plusMinus;
	
	};
}
#endif