//created by Wang Jianwei on 01 Dec 2012
#ifndef FORWARDCURVEBUILDER_H
#define FORWARDCURVEBUILDER_H
#include "AbstractCurve.h"
#include "AbstractBuilder.h"
#include "CcyPair.h"
#include "Market.h"
#include "AbstractCurve.h"
#include <vector>

using namespace instruments;
typedef tuple<date, double> point;

namespace utilities{
	class ForwardCurveBuilder: public AbstractBuilder{
		
	public:
		
		ForwardCurveBuilder():AbstractBuilder(){};
		ForwardCurveBuilder(std::string ccyPair):AbstractBuilder(){
			_ccyPair = CcyPair(ccyPair);
		}

		void init(Configuration* cfg);

		AbstractCurve<date>* build(Configuration* cfg);

		// Getters and Setters
		CcyPair getCcyPair(){ return _ccyPair;}

		void setCcyPair(CcyPair ccyPair){_ccyPair = ccyPair;}

	private:

		void buildSection(AbstractCurve<date>* dc);
		
		CcyPair _ccyPair;
		point _curvePointer;
	};
}
#endif