//created by Jianwei on 16 Dec 2012
#ifndef MATHUTIL_H
#define MATHUTIL_H
#include "Matrix.h"

namespace utilities {
	class MathUtil {

	public:

		static double CNF(double x);
		static double invCNF(double p);
		static Matrix leastSquare(Matrix X, Matrix Y);
		static Matrix restrictedLeastSquare(Matrix X, Matrix Y, Matrix R);
	};
}
#endif