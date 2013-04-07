//created by Jianwei on 30 Nov 2012
#ifndef BOND_H
#define BOND_H
#include "AbstractInstrument.h"
#include "Enums.h"
#include "Market.h"
#include "DiscountCurve.h"
#include "Configuration.h"
#include "cashflowLeg.h"
#include <iostream>

using namespace utilities;
using namespace instruments;
using namespace std;

namespace instruments {
	class  Bond:  public AbstractInstrument{

	public:

		Bond(){
			_instrumentEnum = enums::BOND;
		};
		~Bond(){};

		// Getters
		cashflowLeg* getCouponLeg(){ return &_couponLeg;}
		std::string getCUSIP(){ return _CUSIP; }
		std::string getSecurityType(){ return _securityType; }
		date getFirstCouponDate(){ return _firstCouponDate; }
		date getNextCouponDate(){ return _nextCouponDate; }
		date getPrevCouponDate(){ return _prevCouponDate; }
		double getCouponRate(){ return _couponRate;}
		int getCouponFreq(){ return _couponFreq;}
		int getTenor(){ return _tenorInYear;}
		int getNextCouponIndex(){ return _nextCouponIndex; }
		double getDirtyPrice(){ return _dirtyPrice; }
		double getCleanPrice(){ return _cleanPrice; }
		double getQuotedPrice(){ return _quotedPrice; }
		double getQuotedYTM(){ return _quotedYTM; }
		double getQuotedGSpread(){ return _quotedGSpread; }		
		double getFractionFirstCouponAccrued(){ return _fractionFirstCouponAccrued; }
		bool getIsGeneric(){ return _isGeneric; }
		bool getIsBill(){ return _couponRate==0?true:false; }

		// Setters
		void setCouponLeg(cashflowLeg couponLeg){ _couponLeg = couponLeg; }
		void setCUSIP(std::string CUSIP){ _CUSIP = CUSIP; }
		void setSecurityType(std::string securityType){ _securityType = securityType; }
		void setFirstCouponDate(date firstCouponDate){ _firstCouponDate=firstCouponDate; }
		void setNextCouponDate(date nextCouponDate){ _nextCouponDate=nextCouponDate; }
		void setPrevCouponDate(date prevCouponDate){ _prevCouponDate=prevCouponDate; }
		void setCouponRate(double couponRate){ _couponRate = couponRate;}
		void setCouponFreq(int couponFreq){ _couponFreq = couponFreq;}
		void setTenor(int tenorInYear){ _tenorInYear = tenorInYear;}
		void setNextCouponIndex(int nextCouponIndex){ _nextCouponIndex = nextCouponIndex; }
		void setDirtyPrice(double dirtyPrice){ _dirtyPrice = dirtyPrice;}
		void setCleanPrice(double cleanPrice){ _cleanPrice = cleanPrice;}
		void setQuotedPrice(double quotedPrice){ _quotedPrice = quotedPrice; }
		void setQuotedYTM(double quotedYTM){ _quotedYTM = quotedYTM;}
		void setQuotedGSpread(double quotedGSpread){ _quotedGSpread = quotedGSpread;}
		void setFractionFirstCouponAccrued(double fractionFirstCouponAccrued){ _fractionFirstCouponAccrued = fractionFirstCouponAccrued; }
		void setIsGeneric(bool isGeneric){ _isGeneric = isGeneric; }

		// Methods
		virtual double getMPV(DiscountCurve* dc);
		double getYieldSpread(DiscountCurve* dc);
		double getYield();
		double getZeroRateSpread(DiscountCurve* dc);
		void deriveDirtyPrice();
		void deriveDates();
		void generateCouponLeg();
		date findNextCouponDate();
		void printCouponLeg();
		std::string toString();

	private:

		cashflowLeg _couponLeg;
		date _firstCouponDate;
		date _nextCouponDate;
		date _prevCouponDate;
		std::string _CUSIP;
		std::string _securityType;
		bool _isGeneric;
		bool _isBill;
		double _couponRate;
		int _couponFreq;
		int _tenorInYear;
		int _nextCouponIndex;
		double _dirtyPrice;
		double _cleanPrice;
		double _quotedPrice;
		double _quotedYTM;
		double _quotedGSpread;
		double _fractionFirstCouponAccrued;
		double _accruedInterest;
	};

}
#endif