#include "SwaptionVolCube.h"
#include "InterpolatorFactory.h"
#include "AbstractCurve.h"

using namespace utilities;
using namespace instruments;


typedef tuple<int, double> point;

double SwaptionVolCube::getVol(double strike, double maturityInMonth, int tenorInMonth){
	AbstractCurve<double>* volCurveAlongStrike = getVolCurveAlongStrike(maturityInMonth,tenorInMonth);
	return volCurveAlongStrike->getValue(strike);
}

AbstractCurve<double>* SwaptionVolCube::getVolCurveAlongStrike(double expiryInMonth, int tenorInMonth){
	AbstractCurve<double>* curve = new AbstractCurve<double>();
	map<int, SwaptionVolSurface*>::iterator i;
	map<int, SwaptionVolSurface*>::iterator j;
	for (i = _cube->begin(), j=_cube->begin()++; j!=_cube->end(); i++,j++){
		SwaptionVolSurface* lowerStrikeSurface = (*i).second;
		SwaptionVolSurface* upperStrikeSurface = (*j).second;
		int startStrike =  (*i).first;
		int endStrike =  (*j).first;
		double startVol = lowerStrikeSurface->getValue(tenorInMonth, expiryInMonth);
		double endVol = upperStrikeSurface->getValue(tenorInMonth, expiryInMonth);
		AbstractInterpolator<double>* ai = getVolLineSectionAlongStrike(startStrike, startVol, endStrike, endVol);
		curve->insertLineSection(ai);
	}
	return curve;
}

SwaptionVolSurface* SwaptionVolCube::getVolSurface(int strike){
	map<int, SwaptionVolSurface*>::iterator it;
	for (it = _cube->begin(); it!=_cube->end(); it++)
		if ((*it).first==strike)
			return (*it).second;
	throw "Swaption vol surface not found!";
}

AbstractInterpolator<double>* SwaptionVolCube::getVolLineSectionAlongStrike(int startStrike, double startVol, int endStrike, double endVol){
	point startPoint(startStrike,startVol);
	point endPoint(endStrike,endVol);
	AbstractInterpolator<double>* ai = InterpolatorFactory<double>::getInstance()->getInterpolator(startPoint, endPoint, _interpolAlgo);
	return ai;
}