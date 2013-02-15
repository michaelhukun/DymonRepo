#include "FXSkewSurface.h"

using namespace utilities;

double FXSkewSurface::getValue(long majorAxisVal, double minorAxisVal){
	return 0;
}

string FXSkewSurface::dumpSruface(int deltaAxisPointNum){
	std::stringstream ss (stringstream::in | stringstream::out);
	for (auto it = _curves->begin(); it!=_curves->end(); it++){
		ss<<"Tenor axis ["<<(*it).first<<"]: ";
		AbstractCurve<double>* skew = (*it).second;
		double curveStartX = 0;
		double curveEndX = 1;
		double interval = (curveEndX-curveStartX)/deltaAxisPointNum;
		while(curveStartX<=curveEndX){
			ss << skew->getValue(curveStartX)<<",";
			curveStartX = curveStartX + interval;
		}
		ss<<"\n";
	}
	return ss.str();
}