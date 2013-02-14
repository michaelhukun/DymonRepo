#include "FXSkewBuilder.h"
#include "AbstractCurve.h"
#include "EnumHelper.h"
#include "Enums.h"
#include "DeltaVol.h"
#include "RecordHelper.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"

using namespace utilities;
typedef AbstractBuilder super;

void FXSkewBuilder::init(Configuration* cfg){
	super::init(cfg);
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("FXSkew.interpol",false,"QUADRATIC"));
	_numericAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("FXSkew.numerical",false,"BISECTION"));
	_cutOff = stod(cfg->getProperty("FXSkew.cutoff",false,"1"));
	_iterateCount = std::stoi(cfg->getProperty("numerical.iteration", false, "50"));
	_tolerance = std::stod(cfg->getProperty("numerical.tolerance", false, "0.0000001"));
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
	_deltaVector = getDeltaVector(_ccyPair.toString(), _tenorInYear);
	deriveATMDelta(_deltaVector);
	buildCutOffSection(ac);
}

double FXSkewBuilder::getForeignRate(CcyPair ccyPair){
	enums::CurrencyEnum foreignCcy = ccyPair.getForeignCCY();
	return 0;
}

void FXSkewBuilder::buildQuadraticSection(AbstractCurve<double>* ac){
	double b0 = 1;
	AbstractNumerical<FXSkewBuilder>* an = NumericalFactory<FXSkewBuilder>::getInstance()->getNumerical(this,&FXSkewBuilder::numericalFunc,_numericAlgo);
	double lowerBound = -100;
	double upperBound = 100;
	double b1 = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
	double b2 = b1tob2(b1);
}

void FXSkewBuilder::buildCutOffSection(AbstractCurve<double>* ac){
}

double FXSkewBuilder::numericalFunc(double b1){
	double volSTR25 = getVolFromVector(STR,25.0);
	double volRR25 = getVolFromVector(RR,25.0);
	double volATM = getVolFromVector(ATM, 0);
	double b2 = b1tob2(b1);
	double diff =0.5*b1*volRR25*(1-2*volATM)  +0.5*b2*volSTR25*(pow(0.25-volATM,2) + pow(0.75-volATM,2)) - volSTR25;
	return diff;
}

double FXSkewBuilder::b1tob2(double b1){
	double volSTR25 = getVolFromVector(STR,25.0);
	double volRR25 = getVolFromVector(RR,25.0);
	double volATM = getVolFromVector(ATM, 0);
	double b2 = volRR25*(1+0.5*b1)/volSTR25/(pow(0.25-volATM,2) - pow(0.75-volATM,2));
}

double FXSkewBuilder::getVolFromVector(enums::OptionType optionType, double delta){
	for (unsigned int i=0; i<_deltaVector->size(); i++){
		DeltaVol deltaVol = _deltaVector->at(i);
		if (deltaVol.getOptionType() == ATM){
			return deltaVol.getVol();
		} else if (deltaVol.getOptionType() == optionType && deltaVol.getDelta() == delta){
			return deltaVol.getVol();
		}
	}
	throw "Vol not found!";
}