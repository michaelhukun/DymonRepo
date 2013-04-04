//created by Wang Jianwei on 1 Dec 2012
#include "BondCurveBuilder.h"
#include <iostream>
#include "CashFlowLegBuilder.h"
#include "BondRateBootStrapper.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "EnumHelper.h"
#include "dateUtil.h"
#include "RecordHelper.h"
#include "Constants.h"

using namespace utilities;
typedef AbstractBuilder super;
typedef tuple<date, double> point;

void BondCurveBuilder::init(Configuration* cfg){
	super::init(cfg);
	_curveStartDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,_market.getCurrencyEnum());
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("BondDiscountCurve.interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("BondDiscountCurve.numerical",false,"BISECTION"));
	_interpolRateType = EnumHelper::getRateType(cfg->getProperty("BondDiscountCurve.interpol.rate",true,"ZERO"));
	_bondCurveDayCount = EnumHelper::getDayCountEnum(cfg->getProperty("BondDiscountCurve.daycount",false,"ACT/365"));

}

DiscountCurve* BondCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	_bondMap = RecordHelper::getInstance()->getBondRateMap()->at(_market.getCurrencyEnum());
	DiscountCurve* dc = new DiscountCurve();
	dc->setName(_market.getNameString()+" Bond Curve");
	dc->setDayCount(_bondCurveDayCount);
	dc->setInterpolRateType(_interpolRateType);
	buildSection(dc);
	dc->dumpComponentNames();
	return dc;
}


void BondCurveBuilder::buildSection(DiscountCurve* dc){	
	_curvePointer = point(_curveStartDate,1);
	for (auto it=_bondMap.begin(); it != _bondMap.end(); it++ ){
		Bond* bond = &(it->second);
		int numOfNights = (int) (*it).first;
		//if (bond->getIsGeneric()==false) 
		//	continue;

		vector<cashflow> couponLeg = bond->getCouponLeg()->getCashFlowVector();
		date lastPaymentDate = couponLeg[couponLeg.size()-1].getPaymentDate();
		BondRateBootStrapper bondBS(_curvePointer, lastPaymentDate, bond, dc, _interpolAlgo, _numericalAlgo, _market);
		bondBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = bondBS.bootStrap();
		dc->insertLineSection(lineSection);
		dc->insertComponent(bond);
		_curvePointer = lineSection->getEndPoint();
	}
}
