#include "ForwardCurveBuilder.h"
#include "FXForward.h"
#include "AbstractCurve.h"
#include "EnumHelper.h"
#include "Enums.h"
#include "RecordHelper.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"
#include "LinearInterpolator.h"
#include "LogLinearInterpolator.h"
#include "InterpolatorFactory.h"
#include "AbstractInterpolator.h"

using namespace utilities;
typedef AbstractBuilder super;

void ForwardCurveBuilder::init(Configuration* cfg){
	super::init(cfg);
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("ForwardCurveBuilder.interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("ForwardCurveBuilder.numerical",false,"BISECTION"));
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration", false, "50"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance", false, "0.0000001"));
	
}

AbstractCurve<date>* ForwardCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	AbstractCurve<date>* ac = new AbstractCurve<date>();
   BuildSection(ac);
	return ac;
}


AbstractCurve<date>* ForwardCurveBuilder::BuildSection(AbstractCurve<date>* ac){
   auto ForwardMap = &RecordHelper::getInstance()->getFXForwardMap()->at(_ccyPair.toString());
   map<long, FXForward>::iterator i;
   map<long, FXForward>::iterator j;
   for (i = ForwardMap->begin(), j = ++ForwardMap->begin(); j != ForwardMap->end(); i++,j++) {
      point P1 = point(date(i->first), (i->second).getOutRight());
      point P2 = point(date(j->first), (j->second).getOutRight());
	   AbstractInterpolator<date>* interpolant = InterpolatorFactory<date>::getInstance()->getInterpolator(P1,P2, _interpolAlgo);
	   ac->insertLineSection(interpolant);
   }
   return ac;
}





