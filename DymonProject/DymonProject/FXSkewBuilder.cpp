#include "FXSkewBuilder.h"
#include "AbstractCurve.h"
#include "EnumHelper.h"
#include "Enums.h"
#include "FXEuropeanOption.h"
#include "RecordHelper.h"
#include "AbstractNumerical.h"
#include "NumericalFactory.h"
#include "QuadraticInterpolator.h"
#include "marketdata.h"

using namespace utilities;
using namespace Markets;
typedef AbstractBuilder super;

void FXSkewBuilder::init(Configuration* cfg){
	super::init(cfg);
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("FXSkew.interpol",false,"QUADRATIC"));
	_numericalAlgo = EnumHelper::getNumericalAlgo(cfg->getProperty("FXSkew.numerical",false,"BISECTION"));
	_cutOff = stod(cfg->getProperty("FXSkew.cutoff",false,"0.01"));
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

vector<FXEuropeanOption>* FXSkewBuilder::getOptionVector(std::string ccyPairStr, int daysToExpiry){
	auto deltaMap = &RecordHelper::getInstance()->getFXVolSkewMap()->at(_ccyPair.toString());
	vector<FXEuropeanOption>* deltaVector = &deltaMap->at(daysToExpiry);
	return deltaVector;
}

double FXSkewBuilder::deriveATMDelta(vector<FXEuropeanOption>* optionVector){
	for(unsigned int i=0; i<optionVector->size(); i++){
		FXEuropeanOption* option = &optionVector->at(i);		
		option->deriveDomesticDCF();
		option->deriveForeignDCF();
		if (option->getVolType() == ATM){
			double tenorExpiry = option->getTenorExpiry();
			double volATM = option->getVol();
			if (_ccyPair.toString()=="EURUSD" && tenorExpiry<2)
				option->setDelta(exp(-option->getForeignDCF())/2);
			else if (_ccyPair.toString()=="EURUSD" && tenorExpiry>=2)
				option->setDelta(0.5);
			else if (tenorExpiry<2)
				option->setDelta(exp(-option->getForeignDCF()-pow(volATM,2)*tenorExpiry/2)/2);
			else if (tenorExpiry>=2 || _ccyPair.isEmergingMarket())
				option->setDelta(exp(-pow(volATM,2)*tenorExpiry/2)/2);
			return option->getDelta();
		}
	}
	throw "Delta not found!";
}

void FXSkewBuilder::buildQuadratic(AbstractCurve<double>* ac){
	_optionVector = getOptionVector(_ccyPair.toString(), _daysToExpiry);
	_deltaATM = deriveATMDelta(_optionVector);
	_volSTR25 = getVolFromVector(STR,0.25);
	_volRR25 = getVolFromVector(RR,0.25);
	_volATM = getVolFromVector(ATM,0);
	buildQuadraticSection(ac);
	buildCutOffSection(ac);
}

void FXSkewBuilder::buildQuadraticSection(AbstractCurve<double>* ac){
	double b0 = 1;
	AbstractNumerical<FXSkewBuilder>* an = NumericalFactory<FXSkewBuilder>::getInstance()->getNumerical(this,&FXSkewBuilder::numericalFunc,_numericalAlgo);
	double lowerBound = -10;
	double upperBound = 10;
	double b1 = an->findRoot(lowerBound,upperBound,_tolerance,_iterateCount);
	double b2 = b1tob2(b1);

	double B0 = b0*_volATM-b1*_deltaATM*_volRR25+b2*pow(_deltaATM,2)*_volSTR25;
	double B1 = b1*_volRR25 - 2*b2*_deltaATM*_volSTR25;
	double B2 = b2*_volSTR25;
	QuadraticInterpolator<double>* interpolant = new QuadraticInterpolator<double>(0,1,B0,B1,B2);
	ac->insertLineSection(interpolant);
}

void FXSkewBuilder::buildCutOffSection(AbstractCurve<double>* ac){
	double leftCutOffVal = ac->getValue(_cutOff);
	double rightCutOffVal = ac->getValue(1-_cutOff);
	tuple<double, double> leftCutOffPoint = tuple<double, double>(_cutOff,leftCutOffVal);
	tuple<double, double> rightCutOffPoint = tuple<double, double>(1-_cutOff,rightCutOffVal);
	ac->getSection(0)->setStartPoint(leftCutOffPoint);
	ac->getSection(ac->getSize()-1)->setEndPoint(rightCutOffPoint);
}

double FXSkewBuilder::numericalFunc(double b1){
	double b2 = b1tob2(b1);
	double diff =_volRR25*(-1-0.5*b1)+b2*_volSTR25*(pow(0.25-_deltaATM,2) - pow(0.75-_deltaATM,2));
	return diff;
}

double FXSkewBuilder::b1tob2(double b1){
	double b2 = (2*_volSTR25-b1*_volRR25*(1-2*_deltaATM))/(pow(0.25-_deltaATM,2) + pow(0.75-_deltaATM,2))/_volSTR25;
	return b2;
}

double FXSkewBuilder::getVolFromVector(enums::VolType optionType, double delta){
	for (unsigned int i=0; i<_optionVector->size(); i++){
		FXEuropeanOption option = _optionVector->at(i);
		if (option.getVolType() == ATM && optionType == ATM){
			return option.getVol();
		} else if (option.getVolType() == optionType && option.getDelta() == delta){
			return option.getVol();
		}
	}
	throw "Vol not found!";
}