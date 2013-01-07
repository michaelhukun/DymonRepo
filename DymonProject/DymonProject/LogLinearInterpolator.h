//created by Wang Jianwei on 01 Dec 2012
#ifndef LOGLINEARINTERPOLATOR_H
#define LOGLINEARINTERPOLATOR_H
#include "AbstractInterpolator.h"

namespace utilities{

	template<typename T> class LogLinearInterpolator: public AbstractInterpolator<T>{
		
	typedef tuple<T, double> point;

	public:

		LogLinearInterpolator(point startPoint, point endPoint):
		  AbstractInterpolator(startPoint, endPoint){
			  _slope = NaN;
			  _algo = enums::LOGLINEAR;
		  }

		  virtual point interpolate(T xVal){
			  xValInRangeCheck(xVal);
			  if (_slope == NaN){
				  double startVal = std::get<1>(_startPoint);
				  double endVal = std::get<1>(_endPoint);
				  double yDiff = log(endVal) - log(startVal);
				  double xDiff = std::get<0>(_endPoint) - std::get<0>(_startPoint);
				  _slope = yDiff / xDiff;
			  }
			  double  yVal = _slope*(xVal - std::get<0>(_startPoint))+ log(std::get<1>(_startPoint));
			  return point(xVal,exp(yVal));
		  }

	private:

		double _slope;
	};
}
#endif