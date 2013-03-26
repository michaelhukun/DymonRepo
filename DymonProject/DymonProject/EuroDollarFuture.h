//created by Jianwei on 30 Nov 2012
#ifndef EURODOLLARFUTURE_H
#define EURODOLLARFUTURE_H
#include "AbstractInstrument.h"
#include "Enums.h"
#include "Market.h"
#include "DiscountCurve.h"
#include "Configuration.h"
#include "Future.h"
#include "bond.h"
#include <iostream>

using namespace utilities;
using namespace instruments;
using namespace std;

namespace instruments {
	class EuroDollarFuture: public Future{

	public:

		EuroDollarFuture():Future(){};
		~EuroDollarFuture(){};

		// Getters
		AbstractInstrument* getReset(){ return &_reset; }
		double getConvAdj(){ return _convAdj; }
		double getForwardLiborRate(){ return _forwardLiborRate; }
		
		void setReset(AbstractInstrument reset){ _reset = reset; }
		void setConvAdj(double convAdj){ _convAdj = convAdj; }
		void setForwardLiborRate(double forwardLiborRate){ _forwardLiborRate = forwardLiborRate; }

		// Methods

		void genereateReset();
		date getMaxFutureAndResetDeliveryDate();
		std::string toString(){return "";}

	private:

		AbstractInstrument _reset;
		double _forwardLiborRate;
		double _convAdj;
	};

}
#endif