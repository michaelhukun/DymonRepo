//created by Wang Jianwei on 01 Dec 2012
//Added cashflowleg creating - Kun
#ifndef FXSkewBuilder_H
#define FXSkewBuilder_H
#include "AbstractCurve.h"
#include "AbstractBuilder.h"
#include "CcyPair.h"
#include "Market.h"
#include "FXEuropeanOption.h"
#include <vector>

using namespace instruments;

namespace utilities{
	class FXSkewBuilder: public AbstractBuilder{
		
	public:
		
		FXSkewBuilder():AbstractBuilder(){};
		FXSkewBuilder(std::string ccyPair, int daysToExpiry):AbstractBuilder(){
			_ccyPair = CcyPair(ccyPair);
			_daysToExpiry = daysToExpiry;
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
		vector<FXEuropeanOption>* getOptionVector(std::string ccyPairStr, int daysToExpiry);
		double deriveATMDelta(vector<FXEuropeanOption>* optionVector);
		void buildCutOffSection(AbstractCurve<double>* ac);
		void buildQuadraticSection(AbstractCurve<double>* ac);
		double getVolFromVector(enums::VolType optionType, double delta);

		double _cutOff;
		Market _market;
		CcyPair _ccyPair;
		int _daysToExpiry;
		vector<FXEuropeanOption>* _optionVector;
		int _iterateCount;
		int _daysBeforeSwitchToSwapCurve;
		double _tolerance;
		double _volSTR25;
		double _volRR25;
		double _volATM;
		double _deltaATM;
	};
}
#endif