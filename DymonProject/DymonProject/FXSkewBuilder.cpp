#include "FXSkewBuilder.h"
#include "AbstractCurve.h"
#include "EnumHelper.h"
#include "Enums.h"
#include "DeltaVol.h"
#include "RecordHelper.h"

using namespace utilities;
typedef AbstractBuilder super;

void FXSkewBuilder::init(Configuration* cfg){
	super::init(cfg);
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("FXSkew.interpol",false,"QUADRATIC"));
	_cutOff = stod(cfg->getProperty("FXSkew.cutoff",false,"1"));
}

AbstractCurve<double>* FXSkewBuilder::build(Configuration* cfg){
	if (cfg!=NULL) init(cfg);
	AbstractCurve<double>* ac = new AbstractCurve<double>();
	switch (_interpolAlgo){
	case QUADRATIC:
		buildQuadratic(ac);
		break;
	}
	return ac;
}

vector<DeltaVol>* FXSkewBuilder::getDeltaVector(std::string ccyPairStr, double tenorInYear){
	auto deltaMap = &RecordHelper::getInstance()->getFXVolSkewMap()->at(_ccyPair.toString());
	vector<DeltaVol>* deltaVector = &deltaMap->at(tenorInYear);
	return deltaVector;
}

void FXSkewBuilder::deriveATMDelta(vector<DeltaVol>* deltaVector){
	double foreignRate = getForeignRate(_ccyPair);
	for(unsigned int i=0; i<deltaVector->size(); i++){
		DeltaVol* deltaVol = &deltaVector->at(i);
		if (deltaVol->getOptionType() == ATM){
			double tenorExpiry = deltaVol->getTenorExpiry();
			double tenorDiscount = deltaVol->getTenorDiscount();
			double volATM = deltaVol->getVol();
			if (_ccyPair.toString()=="EURUSD" && tenorExpiry<2)
				deltaVol->setDelta(exp(-foreignRate*tenorDiscount)/2);
			else if (_ccyPair.toString()=="EURUSD" && tenorExpiry>=2)
				deltaVol->setDelta(0.5);
			else if (tenorExpiry<2)
				deltaVol->setDelta(exp(-foreignRate*tenorDiscount-pow(volATM,2)*tenorExpiry/2)/2);
			else if (tenorExpiry>=2 || _ccyPair.isEmergingMarket())
				deltaVol->setDelta(exp(-pow(volATM,2)*tenorExpiry/2)/2);
		}
	}

}

void FXSkewBuilder::buildQuadratic(AbstractCurve<double>* ac){
	vector<DeltaVol>* deltaVector = getDeltaVector(_ccyPair.toString(), _tenorInYear);
	deriveATMDelta(deltaVector);
	buildCutOffSection(ac);
}

double FXSkewBuilder::getForeignRate(CcyPair ccyPair){
	enums::CurrencyEnum foreignCcy = ccyPair.getForeignCCY();
	return 0;
}

void FXSkewBuilder::buildCutOffSection(AbstractCurve<double>* ac){
}