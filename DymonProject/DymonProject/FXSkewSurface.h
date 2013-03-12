//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWSURFACE_H
#define FXSKEWSURFACE_H
#include "AbstractSurface.h"
#include "date.h"
#include "EnumHelper.h"
#include "Configuration.h"

using namespace utilities;

namespace utilities{
	class FXSkewSurface: public AbstractSurface<double, double>{

	public:		

		FXSkewSurface():AbstractSurface(){
			_majorAxisInterpol = EnumHelper::getInterpolAlgo(_cfg->getProperty("FXSkew.tenor.interpol",false,"LINEAR"));
			_interpolateOnVar = _cfg->getProperty("FXSkew.tenor.interpol.base",false,"VAR")=="VAR"?true:false;
		};

		double getValue(double majorAxisVal, double minorAxisVal);	
		std::string dumpSruface(int deltaAxisPointNum);

	private:

		AbstractCurve<double>* getCurveAlongTenor(double delta);
		AbstractInterpolator<double>* getVolLineSectionAlongTenor(double startTenor, double startVol, double endTenor, double endVol);
		AbstractInterpolator<double>* getVarLineSectionAlongTenor(double startTenor, double startVol, double endTenor, double endVol);

		bool _interpolateOnVar;
	};
}
#endif