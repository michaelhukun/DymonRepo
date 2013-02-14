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
		enums::interpolAlgo getInterpolAlgo(){return _interpolAlgo;}
		CcyPair getCcyPair(){ return _ccyPair;}
		double getCutOff(){ return _cutOff; }

		void setMarket(Market market){_market = market;}
		void setInterpolAlgo(enums::interpolAlgo interpolAlgo){_interpolAlgo=interpolAlgo;}
		void setCcyPair(CcyPair ccyPair){_ccyPair = ccyPair;}
		void setCutOff(double cutOff){ _cutOff = cutOff; }

	private:

		void buildQuadratic(AbstractCurve<double>* ac);
		vector<DeltaVol>* getDeltaVector(std::string ccyPairStr, double tenorInYear);
		void deriveATMDelta(vector<DeltaVol>* deltaVector);
		double getForeignRate(CcyPair ccyPair);
		void buildCutOffSection(AbstractCurve<double>* ac);

		double _cutOff;
		Market _market;
		CcyPair _ccyPair;
		double _tenorInYear;
		enums::interpolAlgo _interpolAlgo;
	};
}
#endif