//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWSURFACE_H
#define FXSKEWSURFACE_H
#include "AbstractSurface.h"
#include "date.h"

using namespace utilities;

namespace utilities{
	class FXSkewSurface: public AbstractSurface<double, double>{

	public:		

		FXSkewSurface():AbstractSurface(){};

		double getValue(long majorAxisVal, double minorAxisVal);
	
		std::string dumpSruface(int deltaAxisPointNum);
	};
}
#endif