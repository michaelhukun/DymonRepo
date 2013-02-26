#include "FXSkewSurface.h"
#include "InterpolatorFactory.h"
#include "AbstractCurve.h"
using namespace utilities;

typedef tuple<double, double> point;

double FXSkewSurface::getValue(double tenor, double delta){	
	AbstractCurve<double>* volCurveAlongTenor = getCurveAlongTenor(delta);
	double vol;
	if (_interpolateOnVar){
			vol = volCurveAlongTenor->getValue(tenor);
		} else {
			vol = sqrt(volCurveAlongTenor->getValue(tenor)/tenor);
		}
	return vol;
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

AbstractCurve<double>* FXSkewSurface::getCurveAlongTenor(double delta){
	AbstractCurve<double>* curve = new AbstractCurve<double>();
	map<double, AbstractCurve<double>*>::iterator i;
	map<double, AbstractCurve<double>*>::iterator j;
	for (i = _curves->begin(), j=++_curves->begin(); j!=_curves->end(); i++,j++){
		AbstractCurve<double>* lowerTenorCurve = (*i).second;
		AbstractCurve<double>* upperTenorCurve = (*j).second;
		double startTenor =  (*i).first;
		double endTenor =  (*j).first;
		double startVol = lowerTenorCurve->getValue(delta);
		double endVol = upperTenorCurve->getValue(delta);
		AbstractInterpolator<double>* ai;
		if (_interpolateOnVar){
			ai = getVarLineSectionAlongTenor(startTenor, startVol, endTenor, endVol);
		} else {
			ai = getVolLineSectionAlongTenor(startTenor, startVol, endTenor, endVol);
		}
		curve->insertLineSection(ai);
	}
	return curve;
}

AbstractInterpolator<double>* FXSkewSurface::getVolLineSectionAlongTenor(double startTenor, double startVol, double endTenor, double endVol){
	point startPoint(startTenor,startVol);
	point endPoint(endTenor,endVol);
	AbstractInterpolator<double>* ai = InterpolatorFactory<double>::getInstance()->getInterpolator(startPoint, endPoint, _majorAxisInterpol);
	return ai;
}


AbstractInterpolator<double>* FXSkewSurface::getVarLineSectionAlongTenor(double startTenor, double startVol, double endTenor, double endVol){
	point startPoint(startTenor,startTenor*pow(startVol,2));
	point endPoint(endTenor,endTenor*pow(endVol,2));
	AbstractInterpolator<double>* ai = InterpolatorFactory<double>::getInstance()->getInterpolator(startPoint, endPoint, _majorAxisInterpol);
	return ai;
}