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

using namespace std;
using namespace instruments;
using namespace utilities;

namespace instruments {

	typedef tuple<date,double> PV;
	typedef map<tuple<double,double>,double> volSurfacePoint;
	
	class AbstractPricer {
	
	public:
		//base class for all other instruments to be derived from
		AbstractPricer(){};
		~AbstractPricer(){};		
		
		virtual double getMPV(){return NaN;};

		template <class T, class P> vector<PV> getPVLeg(T aInstrument,DiscountCurve curve, int fixOrFloating);
		
	protected: 
		double _MPV;	
		
	};
}
#endif