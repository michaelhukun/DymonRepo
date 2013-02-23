//created by Wang Jianwei on 01 Dec 2012
#ifndef FXFORWARDIMPLIEDCURVEBUILDER_H
#define FXFORWARDIMPLIEDCURVEBUILDER_H
#include "AbstractCurve.h"
#include "AbstractBuilder.h"
#include "CcyPair.h"
#include "Market.h"
#include "DiscountCurve.h"
#include <vector>

using namespace instruments;
typedef tuple<date, double> point;

namespace utilities{
	class FXForwardImpliedCurveBuilder: public AbstractBuilder{
		
	public:
		
		FXForwardImpliedCurveBuilder():AbstractBuilder(){};
		FXForwardImpliedCurveBuilder(std::string ccyPair):AbstractBuilder(){
			_ccyPair = CcyPair(ccyPair);
		}

		void init(Configuration* cfg);

		DiscountCurve* build(Configuration* cfg);

		// Getters and Setters
		CcyPair getCcyPair(){ return _ccyPair;}
		enums::CurrencyEnum getForwardImpliedCurveCcy(){ return _forwardImpliedCurveCcy; }

		void setCcyPair(CcyPair ccyPair){_ccyPair = ccyPair;}

	private:

		void buildSection(DiscountCurve* dc);
		
		CcyPair _ccyPair;
		point _curvePointer;
		enums::CurrencyEnum _baseYieldCurveCcy;
		enums::CurrencyEnum _forwardImpliedCurveCcy;
	};
}
#endif