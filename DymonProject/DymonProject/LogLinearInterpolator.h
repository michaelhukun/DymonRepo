//created by Wang Jianwei on 01 Dec 2012
#ifndef LOGLINEARINTERPOLATOR_H
#define LOGLINEARINTERPOLATOR_H
#include "AbstractInterpolator.h"

namespace utilities{

	template<typename T> class LogLinearInterpolator: public AbstractInterpolator<T>{

		typedef tuple<T, double> Point;

	public:

		LogLinearInterpolator(Point startPoint, Point endPoint):
		  AbstractInterpolator(startPoint, endPoint){
			  _slope = NaN;
			  _algo = enums::LOGLINEAR;
		  }

		  Point interpolateConverted(Point convertedStartPoint, Point convertedEndPoint, T xVal){
			  if (_slope == NaN){
				  double startVal = std::get<1>(convertedStartPoint);
				  double endVal = std::get<1>(convertedEndPoint);
				  double yDiff = log(endVal) - log(startVal);
				  double xDiff = std::get<0>(convertedEndPoint) - std::get<0>(convertedStartPoint);
				  _slope = yDiff / xDiff;
			  }
			  double  yVal = _slope*(xVal - std::get<0>(convertedStartPoint))+ log(std::get<1>(convertedStartPoint));
			  return Point(xVal,exp(yVal));
		  }

	private:

		double _slope;
	};
}
#endif