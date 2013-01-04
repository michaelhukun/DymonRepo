#include "SwaptionVolCube.h"
#include "InterpolatorFactory.h"
#include "AbstractCurve.h"

using namespace utilities;
using namespace instruments;


typedef map<double, SwaptionVolSurface*>::iterator CubeIterator;
typedef tuple<double, double> point;

double SwaptionVolCube::getVol(double strike, double maturityInMonth, double tenorInMonth){
	AbstractCurve<double>* volCurveAlongStrike = getVolCurveAlongStrike(maturityInMonth,tenorInMonth);
	return volCurveAlongStrike->getValue(strike);
}

AbstractCurve<double>* SwaptionVolCube::getVolCurveAlongStrike(double maturityInMonth, double tenorInMonth){
	AbstractCurve<double>* curve = new AbstractCurve<double>();
	for (CubeIterator i = _cube->begin(), j=_cube->begin()++; j!=_cube->end(); i++,j++){
		SwaptionVolSurface* lowerStrikeSurface = (*i).second;
		SwaptionVolSurface* upperStrikeSurface = (*j).second;
		double startStrike =  (*i).first;
		double endStrike =  (*j).first;
		double startVol = lowerStrikeSurface->getValue(maturityInMonth,tenorInMonth);
		double endVol = upperStrikeSurface->getValue(maturityInMonth,tenorInMonth);
		AbstractInterpolator<double>* ai = getVolLineSectionAlongStrike(startStrike, startVol, endStrike, endVol);
		curve->insertLineSection(ai);
	}
	return curve;
}

SwaptionVolSurface* SwaptionVolCube::getVolSurface(double strike){
	for (map<double, SwaptionVolSurface*>::iterator it = _cube->begin(); it!=_cube->end(); it++)
		if ((*it).first==strike)
			return (*it).second;
	throw "Swaption vol surface not found!";
}

AbstractInterpolator<double>* SwaptionVolCube::getVolLineSectionAlongStrike(double startStrike, double startVol, double endStrike, double endVol){
	point startPoint(startStrike,startVol);
	point endPoint(endStrike,endVol);
	AbstractInterpolator<double>* ai = InterpolatorFactory<double>::getInstance()->getInterpolator(startPoint, endPoint, _interpolAlgo);
	return ai;
}