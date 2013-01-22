//created by Jianwei on 30 Nov 2012
#ifndef BONDFUTURE_H
#define BONDFUTURE_H
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
	class BondFuture: public Future{

	public:

		BondFuture(){};
		~BondFuture(){};

		// Getters
		int getNotionalBondTenor(){ return _notionalBondTenor; }
		double getNotionalBondCouponRate(){ return _notionalBondCouponRate; }
		int getContractSize(){ return _contractSize; }
		date getFirstTradeDate(){ return _firstTradeDate;}
		date getLastTradeDate(){ return _lastTradeDate;}
		date getFirstDeliverDate(){ return _firstDeliverDate;}
		date getLastDeliverDate(){ return _lastDeliverDate;}
		Bond* getMostLikelyBond(){ return &_mostLikelyBond; }

		// Setters
		void setNotionalBondTenor(int notionalBondTenor){ _notionalBondTenor = notionalBondTenor; }
		void setNotionalBondCouponRate(double notionalBondCouponRate){  _notionalBondCouponRate = notionalBondCouponRate; }
		void setContractSize(int contractSize){  _contractSize = contractSize; }
		void setFirstTradeDate(date firstTradeDate){  _firstTradeDate = firstTradeDate;}
		void setLastTradeDate(date lastTradeDate){  _lastTradeDate = lastTradeDate;}
		void setFirstDeliverDate(date firstDeliverDate){  _firstDeliverDate = firstDeliverDate;}
		void setLastDeliverDate(date lastDeliverDate){  _lastDeliverDate = _lastDeliverDate;}
		void setMostLikelyBond(Bond mostLikelyBond){  _mostLikelyBond = mostLikelyBond; }

		// Methods
		double getMPV(DiscountCurve* dc);

	private:

		int _notionalBondTenor;
		double _notionalBondCouponRate;
		int _contractSize;
		date _firstTradeDate;
		date _lastTradeDate;
		date _firstDeliverDate;
		date _lastDeliverDate;
		Bond _mostLikelyBond;
	};

}
#endif