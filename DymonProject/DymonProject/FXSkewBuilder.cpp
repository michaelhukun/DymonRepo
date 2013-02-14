#include "FXSkewBuilder.h"
#include "AbstractCurve.h"
#include "EnumHelper.h"
#include "Enums.h"

using namespace utilities;
typedef AbstractBuilder super;

void FXSkewBuilder::init(Configuration* cfg){
	super::init(cfg);
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("FXSkew.interpol",false,"QUADRATIC"));
}

AbstractCurve<double>* FXSkewBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	AbstractCurve<double>* ac = new AbstractCurve<double>();
	if (_interpolAlgo == QUADRATIC)
		buildQuadratic(ac);
	return ac;
}

void FXSkewBuilder::buildQuadratic(AbstractCurve<double>* dc){
}
