//created by Wang Jianwei on 01 Dec 2012
//Added cashflowleg creating - Kun
#ifndef FXSkewBuilder_H
#define FXSkewBuilder_H
#include "AbstractCurve.h"
#include "AbstractBuilder.h"
#include "CcyPair.h"
#include "Market.h"
#include "DeltaVol.h"
#include <vector>

using namespace instruments;

namespace utilities{
	class FXSkewBuilder: public AbstractBuilder{
		
	public:
		
		FXSkewBuilder():AbstractBuilder(){};
		FXSkewBuilder(std::string ccyPair, double tenorInYear):AbstractBuilder(){
			_ccyPair = CcyPair(ccyPair);
			_tenorInYear = tenorInYear;
		}

		void init(Configuration* cfg);

		AbstractCurve<double>* build(Configuration* cfg);

		// Getters and Setters
		Market getMarket(){return _market;}
		CcyPair getCcyPair(){ return _ccyPair;}
		double getCutOff(){ return _cutOff; }

		void setMarket(Market market){_market = market;}
		void setCcyPair(CcyPair ccyPair){_ccyPair = ccyPair;}
		void setCutOff(double cutOff){ _cutOff = cutOff; }

	private:

		void buildQuadratic(AbstractCurve<double>* ac);
		vector<DeltaVol>* getDeltaVector(std::string ccyPairStr, double tenorInYear);
		double deriveATMDelta(vector<DeltaVol>* deltaVector);
		double getForeignRate(CcyPair ccyPair);
		void buildCutOffSection(AbstractCurve<double>* ac);
		void buildQuadraticSection(AbstractCurve<double>* ac);
		double numericalFunc(double b1);
		double b1tob2(double b1);
		double getVolFromVector(enums::OptionType optionType, double delta);

		double _cutOff;
		Market _market;
		CcyPair _ccyPair;
		double _tenorInYear;
		enums::interpolAlgo _interpolAlgo;
		enums::NumericAlgo _numericAlgo;
		vector<DeltaVol>* _deltaVector;
		int _iterateCount;
		double _tolerance;
		double _volSTR25;
		double _volRR25;
		double _volATM;
		double _deltaATM;
	};
}
#endif