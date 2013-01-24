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

		Bond(){};
		~Bond(){};

		// Getters
		Market getMarket(){ return _market; }
		cashflowLeg* getCouponLeg(){ return _couponLeg;}
		std::string getName(){ return _name; }
		std::string getCUSIP(){ return _CUSIP; }
		std::string getSecurityType(){ return _securityType; }
		date getFirstCouponDate(){ return _firstCouponDate; }
		date getNextCouponDate(){ return _nextCouponDate; }
		date getPrevCouponDate(){ return _prevCouponDate; }
		double getCouponRate(){ return _couponRate;}
		int getCouponFreq(){ return _couponFreq;}
		int getTenor(){ return _tenorNumOfMonths;}
		int getNextCouponIndex(){ return _nextCouponIndex; }
		double getDirtyPrice(){ return _dirtyPrice; }
		double getCleanPrice(){ return _cleanPrice; }
		double getQuotedYTM(){ return _quotedYTM; }
		double getFractionFirstCouponAccrued(){ return _fractionFirstCouponAccrued; }
		enums::DayCountEnum getDayCount(){ return _dayCount; }
		bool getIsGeneric(){ return _isGeneric; }

		// Setters
		void setMarket(Market market){ _market = market; }
		void setCouponLeg(cashflowLeg* couponLeg){ _couponLeg = couponLeg; }
		void setName(std::string name){ _name = name; }
		void setCUSIP(std::string CUSIP){ _CUSIP = CUSIP; }
		void setSecurityType(std::string securityType){ _securityType = securityType; }
		void setFirstCouponDate(date firstCouponDate){ _firstCouponDate=firstCouponDate; }
		void setNextCouponDate(date nextCouponDate){ _nextCouponDate=nextCouponDate; }
		void setPrevCouponDate(date prevCouponDate){ _prevCouponDate=prevCouponDate; }
		void setCouponRate(double couponRate){ _couponRate = couponRate;}
		void setCouponFreq(int couponFreq){ _couponFreq = couponFreq;}
		void setTenor(int tenorNumOfMonths){ _tenorNumOfMonths = tenorNumOfMonths;}
		void setNextCouponIndex(int nextCouponIndex){ _nextCouponIndex = nextCouponIndex; }
		void setDirtyPrice(double dirtyPrice){ _dirtyPrice = dirtyPrice;}
		void setCleanPrice(double cleanPrice){ _cleanPrice = cleanPrice;}
		void setQuotedYTM(double quotedYTM){ _quotedYTM = quotedYTM;}
		void setFractionFirstCouponAccrued(double fractionFirstCouponAccrued){ _fractionFirstCouponAccrued = fractionFirstCouponAccrued; }
		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}
		void setIsGeneric(bool isGeneric){ _isGeneric = isGeneric; }

		// Methods
		virtual double getMPV(DiscountCurve* bc);
		double getGspread(DiscountCurve* bc);
		double getYield();
		void deriveDirtyPrice();
		void generateCouponLeg();
		date findNextCouponDate();
		void printCouponLeg();
		std::string toString();

	private:

		cashflowLeg* _couponLeg;
		date _firstCouponDate;
		date _nextCouponDate;
		date _prevCouponDate;
		Market _market;
		std::string _name;
		std::string _CUSIP;
		std::string _securityType;
		bool _isGeneric;
		double _couponRate;
		int _couponFreq;
		int _tenorNumOfMonths;
		int _nextCouponIndex;
		double _dirtyPrice;
		double _cleanPrice;
		double _quotedYTM;
		double _fractionFirstCouponAccrued;
		enums::DayCountEnum _dayCount;
	};

}
#endif