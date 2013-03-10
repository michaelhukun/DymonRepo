//created by Hu Kun on 16 Dec 2012
#ifndef ABSTRACTPRICER_H
#define ABSTRACTPRICER_H
#include "Market.h"
#include "date.h"
#include <vector>
#include <tuple>
#include <map>
#include "DiscountCurve.h"
#include "Constants.h"
#include "AbstractSession.h"

using namespace std;
using namespace instruments;
using namespace utilities;
using namespace Session;

namespace instruments {
	
	class AbstractPricer: AbstractSession{
	
	public:
		AbstractPricer(){};
		~AbstractPricer(){};		
		
      virtual void init(Configuration*)=0;

		virtual double getMPV()=0;

		// Getters and Setters
		DiscountCurve* getDiscountCurve(){ return _discountCurve; }

		void setDiscountCurve(DiscountCurve* discountCurve){ _discountCurve = discountCurve; }
		
	protected: 
		double _MPV;	
		DiscountCurve* _discountCurve;
		
	};
}
#endif