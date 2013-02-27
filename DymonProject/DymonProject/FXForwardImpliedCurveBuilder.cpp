#include "FXForwardImpliedCurveBuilder.h"
#include "FXForwardImpliedBootStrapper.h"
#include "EnumHelper.h"
#include "RecordHelper.h"

using namespace utilities;
using namespace enums;

typedef AbstractBuilder super;
typedef tuple<date, double> point;

void FXForwardImpliedCurveBuilder::init(Configuration* cfg){
	super::init(cfg);
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("FXForwardImplied.interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("FXForwardImplied.numerical",false,"BISECTION"));
	_baseYieldCurveCcy = EnumHelper::getCcyEnum(cfg->getProperty("FXForwardImplied.baseccy",false,"USD"));
	if( _ccyPair.getCCY1Enum()==_baseYieldCurveCcy){
		_forwardImpliedCurveCcy = _ccyPair.getCCY2Enum();
	}else if (_ccyPair.getCCY2Enum()==_baseYieldCurveCcy){
		_forwardImpliedCurveCcy = _ccyPair.getCCY1Enum();
	}else{
		throw "Indicated base yield curve currency mis-match";
	}
}

DiscountCurve* FXForwardImpliedCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	DiscountCurve* dc = new DiscountCurve();
   dc->setName(Market(_forwardImpliedCurveCcy).getNameString()+" Forward Implied Curve");
	buildSection(dc);
	return dc;
}

void FXForwardImpliedCurveBuilder::buildSection(DiscountCurve* dc){
	auto FXForwardMap = RecordHelper::getInstance()->getFXForwardMap()->at(_ccyPair.getCcyPairStr());
	for (auto it=FXForwardMap.begin(); it != FXForwardMap.end(); it++ ){
		date deliveryDate = date(it->first);
		FXForward* forward = &(it->second);		

		if (it == FXForwardMap.begin()){
			_curvePointer = point(forward->getSpotDate(),1);
		}

		FXForwardImpliedBootStrapper bootStrapper(_curvePointer, deliveryDate, forward, _interpolAlgo, _numericalAlgo);
		bootStrapper.setBaseYieldCurveCcy(_baseYieldCurveCcy);
		bootStrapper.setForwardImpliedCurveCcy(_forwardImpliedCurveCcy);
		bootStrapper.init(Configuration::getInstance());
		
		AbstractInterpolator<date>* lineSection = bootStrapper.bootStrap();
		dc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();
	}
}

// fwd = spot * DF(for) / DF(dom)
// spot date -> delivery date
// delta = BS Delta - Premian
