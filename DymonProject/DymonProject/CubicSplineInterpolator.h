//created by Wang Jianwei on 01 Dec 2012
#ifndef CUBICSPLINEINTERPOLATOR_H
#define CUBICSPLINEINTERPOLATOR_H
#include "AbstractInterpolator.h"
#include "Constants.h"

namespace utilities{	

	template<typename T> class CubicSplineInterpolator: public AbstractInterpolator<T>{
		
	typedef tuple<T, double> point;

	public:

		CubicSplineInterpolator(point startPoint, point endPoint):AbstractInterpolator(startPoint, endPoint){
			b1 = NaN;
			b2 = NaN;
			b3 = NaN;
			b4 = NaN;
			_algo = enums::CUBIC;
		}

		virtual point interpolate(T xVal){
			xValInRangeCheck(xVal);
			coefficientCheck();

			double yVal = b1 + b2 * xVal + b3 * xVal^2 + b4 * xVal^3;
			return point(xVal, yVal);
		}

		void coefficientCheck(){
			if (b1==NaN||b2==NaN||b3==NaN||b4==NaN)
				throw "Cubic spline coefficient not set!";
		}

	private:

		double b1;
		double b2;
		double b3;
		double b4;
	};
}
#endif