//created by Jianwei
#ifndef EURODOLLARFUTURERICER_H
#define EURODOLLARFUTURERICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "DiscountCurve.h"
#include "cashflowLeg.h"
#include "Configuration.h"
#include "Future.h"
#include "FuturePricer.h"

using namespace std;
using namespace instruments;

namespace instruments {
	class EuroDollarFuturePricer: public FuturePricer {
	
	public:
		
		EuroDollarFuturePricer(){};
		EuroDollarFuturePricer(Future* future){ _future = future; }
		~EuroDollarFuturePricer(){};

		void init(Configuration* cfg){ FuturePricer::init(cfg); }

		double getMPV();

		double deriveConvAdjFwdLibor();
		
		double deriveForwardLibor();

	private:

		Future* _future;
	};
}
#endif