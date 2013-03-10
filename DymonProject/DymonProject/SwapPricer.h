//created by Hu Kun on 16 Dec 2012
#ifndef SWAPPRICER_H
#define SWAPPRICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "cashflowLeg.h"
#include "Swap.h"

using namespace std;
using namespace instruments;

namespace instruments {
	class SwapPricer: public AbstractPricer {

	public:

		SwapPricer(){};
		SwapPricer(Swap* swap){ _swap = swap; }
		~SwapPricer(){};


		void init(Configuration*){}

		virtual double deriveSwapRate();

		virtual double getMPV();

	protected:
		virtual double getFixLegPV();
		virtual double getFloatLegPV();

	private: 
		Swap* _swap;
	};
}
#endif