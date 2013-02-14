//created by Wang Jianwei on 01 Dec 2012
#ifndef LINEARINTERPOLATOR_H
#define LINEARINTERPOLATOR_H
#include "AbstractInterpolator.h"
#include "Constants.h"

namespace utilities{	

	template<typename T> class LinearInterpolator: public AbstractInterpolator<T>{
		
	typedef tuple<T, double> point;

	public:

		LinearInterpolator(point startPoint, point endPoint):AbstractInterpolator(startPoint, endPoint){
			_b1 = NaN;
			_b2 = NaN;
			_b3 = NaN;
			_algo = enums::QUADRATIC;
		}

		virtual point interpolate(T xVal){
			xValInRangeCheck(xVal);
			if (_b1 == NaN || _b2 == NaN || _b3 == NaN){
				throw "Parameters not initialized!";
			}
			double yVal = _b1 + _b2 * xVal + _b3 * xVal^2;
			return point(xVal, yVal);
		}

		void setB1(double b1){_b1=b1);
		void setB2(double b2){_b2=b2);
		void setB3(double b3){_b3=b3);

	private:

		double _b1;
		double _b2;
		double _b3;
	};
}
#endif