#include "FXSkewSurfaceBuilder.h"
#include "FXSkewBuilder.h"
#include "Constants.h"
#include "Configuration.h"
#include "AbstractCurve.h"
#include "DeltaVol.h"
#include "CcyPair.h"

using namespace utilities;
typedef AbstractBuilder super;

void FXSkewSurfaceBuilder::init(Configuration* cfg){
	super::init(cfg);
}

FXSkewSurface* FXSkewSurfaceBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	FXSkewSurface* surface = new FXSkewSurface();

	map<double, vector<DeltaVol>> deltaMap = RecordHelper::getInstance()->getFXVolSkewMap()->at(_ccyPair.toString());
	for (auto it=deltaMap.begin(); it!=deltaMap.end(); it++){
		double tenorInYear = it->first;
		FXSkewBuilder skewBuilder = FXSkewBuilder(_ccyPair.toString(), tenorInYear);
		AbstractCurve<double>* skew = skewBuilder.build(cfg);
		surface->insertcurve(tenorInYear, skew);
	}
	return surface;
}