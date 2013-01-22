//created by Wang Jianwei on 1 Dec 2012
#include "BondCurveBuilder.h"
#include <iostream>
#include "BuilderCashFlowLeg.h"
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
	
	_market = Market(EnumHelper::getCcyEnum("USD"));
	_curveStartDate = dateUtil::dayRollAdjust(dateUtil::getToday(),enums::Following,_market.getMarketEnum());
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("BondDiscountCurve."+_market.getNameString()+".interpol",false,"LINEAR"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("BondDiscountCurve."+_market.getNameString()+".numerical",false,"BISECTION"));
}

DiscountCurve* BondCurveBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	DiscountCurve* bc = new DiscountCurve();
	buildSection(bc);
	return bc;
}


void BondCurveBuilder::buildSection(DiscountCurve* dc){
	point lineStartPoint(_curveStartDate,1);
	_curvePointer = lineStartPoint;
	map<long,Bond> rateMap = RecordHelper::getInstance()->getBondRateMap()->at(enums::USD);
	for (map<long,Bond>::iterator it=rateMap.begin(); it != rateMap.end(); it++ ){
		Bond bond = (*it).second;
		int numOfNights = (int) (*it).first;
		if (bond.isGeneric()==false) 
			continue;

		vector<cashflow> couponLeg = bond.getCouponLeg()->getCashFlowVector();
		date lastPaymentDate = couponLeg[couponLeg.size()-1].getPaymentDate();
		BondRateBootStrapper bondBS(_curvePointer, lastPaymentDate, bond, dc, _interpolAlgo, _numericalAlgo, _market);
		bondBS.init(Configuration::getInstance());
		AbstractInterpolator<date>* lineSection = bondBS.bootStrap();
		dc->insertLineSection(lineSection);
		_curvePointer = lineSection->getEndPoint();
	}
}
