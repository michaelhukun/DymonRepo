//created by Hu Kun on 29 Nov 2012
#ifndef MARKETDATA_H
#define MARKETDATA_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "instrumentValue.h"
#include "AbstractInterpolator.h"
#include <map>
#include <utility>
#include <tuple>
#include "Enums.h"

using namespace utilities;
using namespace instruments;
using namespace std;
using namespace enums;

namespace markets {

	// Singlton class for marketdata
	class marketdata {
	public:
		static marketdata* getInstance();
		double getPrice();
		void setPrice(double aPrice);

		template <class T>
		map<double,vector<T>> getDiscountCurve(vector<T> inputPoints, date startTenor, date endTenor, enums::interpolAlgo algo);
		
		template <class T>
		map<double,tuple<T,T>> getVolSurface(double underlyingPrice, vector<double> delta, date tradeDate, vector<date> maturity);
		
		template <class T>
		map<double,tuple<T,T,T>> swaptionVolSurface(double underlyingPrice, vector<double> delta, date tradeDate, vector<date> swapMaturity, vector<date> optionMaturity);

		template <class T>
		map<double,vector<T>> getBondCurve(vector<T> inputPoints, date startTenor, date endTenor, enums::interpolAlgo algo);

		template <class T>
		map<double,vector<T>> getFxForwardCurve(vector<T> inputPoints, date startTenor, date endTenor, enums::interpolAlgo algo);

		template <class T>
		map<double,vector<T>> getImpliedDiscountCurve(vector<T> inputPoints, date startTenor, date endTenor, enums::interpolAlgo algo);

	protected:
		//private copy constructor
		marketdata();
		~marketdata();
		
	private:
		static bool instanceFlag;
		static marketdata *single;
		double price;
		
	};
}
#endif