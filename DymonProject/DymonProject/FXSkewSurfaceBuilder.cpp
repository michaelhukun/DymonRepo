#include "FXSkewSurfaceBuilder.h"
#include "FXSkewBuilder.h"
#include "Constants.h"
#include "Configuration.h"
#include "AbstractCurve.h"
#include "CcyPair.h"

using namespace utilities;
typedef AbstractBuilder super;

void FXSkewSurfaceBuilder::init(Configuration* cfg){
	super::init(cfg);
}

FXSkewSurface* FXSkewSurfaceBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	FXSkewSurface* surface = new FXSkewSurface();

	auto deltaMap = RecordHelper::getInstance()->getFXVolSkewMap()->at(_ccyPair.toString());
	for (auto it=deltaMap.begin(); it!=deltaMap.end(); it++){
		int daysToExpiry = it->first;
		FXSkewBuilder skewBuilder = FXSkewBuilder(_ccyPair.toString(), daysToExpiry);
		AbstractCurve<double>* skew = skewBuilder.build(cfg);
		surface->insertcurve(daysToExpiry, skew);
	}
	return surface;
}