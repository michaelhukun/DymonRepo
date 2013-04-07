//created by Wang Jianwei on 01 Dec 2012
#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H
#include "AbstractInterpolator.h"
#include "Constants.h"

namespace utilities{	

	template<typename T> class LinearInterpolator: public AbstractInterpolator<T>{
		
	typedef tuple<T, double> Point;

	public:

		LinearInterpolator(Point startPoint, Point endPoint):AbstractInterpolator(startPoint, endPoint){
			_slope = NaN;
			_algo = enums::LINEAR;
		}

		virtual Point interpolate(T xVal){
			xValInRangeCheck(xVal);
			if (xVal == std::get<0>(_startPoint)) return _startPoint;
			if (xVal == std::get<0>(_endPoint)) return _endPoint;
			Point convertedStartPoint = convertToInterpolRate(_startPoint);
			Point convertedEndPoint = convertToInterpolRate(_endPoint);
			Point convertedMidPoint = interpolateConverted(convertedStartPoint, convertedEndPoint, xVal);
			Point curveMidPoint = convertToCurveRate(convertedMidPoint);
			return curveMidPoint;
		}

		Point interpolateConverted(Point convertedStartPoint, Point convertedEndPoint, T xVal){
			if (_slope == NaN){
				double yDiff = std::get<1>(convertedEndPoint) - std::get<1>(convertedStartPoint);
				double xDiff = std::get<0>(convertedEndPoint) - std::get<0>(convertedStartPoint);
				_slope = yDiff / xDiff;
			}
			double yVal = _slope*(xVal - std::get<0>(convertedStartPoint))+std::get<1>(convertedStartPoint);
			return Point(xVal, yVal);
		}

	private:

		double _slope;
	};
}
#endif