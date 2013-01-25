//created by Jianwei
#ifndef FUTURERICER_H
#define FUTURERICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "DiscountCurve.h"
#include "cashflowLeg.h"
#include "Configuration.h"
#include "Future.h"

using namespace std;
using namespace instruments;

namespace instruments {
	class FuturePricer: public AbstractPricer {
	
	public:
		
		FuturePricer(){};
		FuturePricer(Future* future){ _future = future; }
		~FuturePricer(){};

		void init(Configuration* cfg);
						
		virtual double getMPV(double costOfCarryRate);

	private:
	
		Future* _future;
	};
}
#endif