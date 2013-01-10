#include "SwaptionVolCubeBuilder.h"
#include "EnumHelper.h"
#include "RecordHelper.h"
#include "SwaptionVolSurface.h"
#include "InterpolatorFactory.h"

using namespace utilities;
typedef AbstractBuilder super;

typedef tuple<int, double> point;

void SwaptionVolCubeBuilder::init(Configuration* cfg){
	super::init(cfg);
	
	_market = Market(EnumHelper::getCcyEnum("USD"));
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("swaptionVolCube.usd.interpol",false,"LINEAR"));
}

SwaptionVolCube* SwaptionVolCubeBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	SwaptionVolCube* svc = new SwaptionVolCube();
	RecordHelper::SwaptionCubeMap volCubeMap = RecordHelper::getInstance()->getSwaptionVolMap();
	RecordHelper::SwaptionCubeMap::iterator cubeIt;
	for (cubeIt=volCubeMap.begin();cubeIt!=volCubeMap.end();cubeIt++){
		int strikeDiffATM = (*cubeIt).first;
		RecordHelper::SwaptionSurfaceMap volSurfaceMap = (*cubeIt).second;

		SwaptionVolSurface* volSurface = buildVolSurface(volSurfaceMap);
		svc->insertVolSurface(strikeDiffATM,volSurface);
	}

	return svc;
}

SwaptionVolSurface* SwaptionVolCubeBuilder::buildVolSurface(RecordHelper::SwaptionSurfaceMap volSurfaceMap){
	RecordHelper::SwaptionSurfaceMap::iterator surfaceIt;
	SwaptionVolSurface* volSurface = new SwaptionVolSurface();
	for (surfaceIt=volSurfaceMap.begin();surfaceIt!=volSurfaceMap.end();surfaceIt++){
		int tenor = surfaceIt->first;
		map<int,double> curveMap = surfaceIt->second;

		AbstractCurve<double>* volCurve = buildVolCurve(curveMap);
		volSurface->insertcurve(tenor, volCurve);
	}
	return volSurface;
}

AbstractCurve<double>* SwaptionVolCubeBuilder::buildVolCurve(map<int,double> curveMap){
	AbstractCurve<double>* volCurve = new AbstractCurve<double>();

	map<int,double>::iterator i;
	map<int,double>::iterator j;
	for (i = curveMap.begin(), j=++curveMap.begin(); j!=curveMap.end(); i++,j++){
		int startExpiry = (*i).first;
		int endExpiry = (*j).first;
		double startVol =  (*i).second;
		double endVol =  (*j).second;
		if (startVol!=NaN && endVol!=NaN){
			point startPoint(startExpiry, startVol);
			point endPoint(endExpiry, endVol);
			AbstractInterpolator<double>* ai = InterpolatorFactory<double>::getInstance()->getInterpolator(startPoint, endPoint,_interpolAlgo);
			volCurve->insertLineSection(ai);
		}
	}
	return volCurve;
}