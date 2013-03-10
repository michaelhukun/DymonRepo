//created by Hu Kun on 16 Dec 2012
#ifndef DEPOSITPRICER_H
#define DEPOSITPRICER_H
#include "Market.h"
#include "date.h"
#include "AbstractPricer.h"
#include "Deposit.h"
#include "DiscountCurve.h"

using namespace std;
using namespace instruments;

namespace instruments {
	class DepositPricer: public AbstractPricer {

	public:

		DepositPricer(){};
		DepositPricer(Deposit* deposit){ _deposit = deposit; }
		~DepositPricer(){};

		void init(Configuration*){}

		virtual double getMPV();

		double deriveDepositRate();

	private: 
		Deposit* _deposit;	
	};
}
#endif