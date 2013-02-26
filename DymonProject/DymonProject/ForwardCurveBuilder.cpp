#include "ForwardCurveBuilder.h"
#include "AbstractCurve.h"
#include "EnumHelper.h"
#include "Enums.h"
#include "RecordHelper.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"
#include "QuadraticInterpolator.h"

using namespace utilities;

void ForwardCurveBuilder::init(Configuration* cfg){
}

AbstractCurve<date>* ForwardCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	AbstractCurve<double>* ac = new AbstractCurve<double>();
	switch (_interpolAlgo){
	case QUADRATIC:
		buildQuadratic(ac);
		break;
	}
	return ac;
}