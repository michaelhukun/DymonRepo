//created by Hu Kun on 16 Dec 2012
#ifndef ABSTRACTPRICER_H
#define ABSTRACTPRICER_H
#include "Market.h"
#include "date.h"
#include <vector>
#include <tuple>
#include <map>
#include "DiscountCurve.h"
#define NaN -9999999 

using namespace std;
using namespace instruments;
using namespace utilities;

namespace instruments {
	//tuple<date,double>
	typedef tuple<date,double> PV;
	//typedef map<tuple<date,date>,double> FWDR;

	//map<tuple<tenorNumofMonths,delta>, volatility>
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