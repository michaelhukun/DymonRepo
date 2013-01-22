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
		std::string getType(){ return _type; }
		std::string getShortCut(){ return _shortCut; }
		std::string getCollateralType(){ return _collateralType; }
		date getFirstCouponDate(){ return _firstCouponDate; }
		double getCouponRate(){ return _couponRate;}
		int getCouponFreq(){ return _couponFreq;}
		int getTenor(){ return _tenorNumOfMonths;}
		int getFirstCouponIndex(){ return _firstCouponIndex; }
		double getDirtyPrice(){ return _dirtyPrice; }
		double getCleanPrice(){ return _cleanPrice; }
		double getYTM(){ return _YTM; }
		double getFractionFirstCouponAccrued(){ return _fractionFirstCouponAccrued; }
		enums::DayCountEnum getDayCount(){ return _dayCount; }

		// Setters
		void setMarket(Market market){ _market = market; }
		void setCouponLeg(cashflowLeg* couponLeg){ _couponLeg = couponLeg; }
		void setName(std::string name){ _name = name; }
		void setType(std::string type){ _type = type; }
		void setShortCut(std::string shortCut){ _shortCut = shortCut; }
		void setCollateralType(std::string colleralType){ _collateralType = colleralType; }
		void setFirstCouponDate(date firstCouponDate){ _firstCouponDate=firstCouponDate; }
		void setCouponRate(double couponRate){ _couponRate = couponRate;}
		void setCouponFreq(int couponFreq){ _couponFreq = couponFreq;}
		void setTenor(int tenorNumOfMonths){ _tenorNumOfMonths = tenorNumOfMonths;}
		void setFirstCouponIndex(int firstCouponIndex){ _firstCouponIndex = firstCouponIndex; }
		void setDirtyPrice(double dirtyPrice){ _dirtyPrice = dirtyPrice;}
		void setCleanPrice(double cleanPrice){ _cleanPrice = cleanPrice;}
		void setYTM(double YTM){ _YTM = YTM;}
		void setFractionFirstCouponAccrued(double fractionFirstCouponAccrued){ _fractionFirstCouponAccrued = fractionFirstCouponAccrued; }
		void setDayCount(enums::DayCountEnum dayCount){ _dayCount = dayCount;}

		// Methods
		bool isGeneric();
		virtual double getMPV(DiscountCurve* bc);
		double getGspread(DiscountCurve* bc);
		double getYield();
		void deriveDirtyPrice();
		void generateCouponLeg();
		date findFirstCouponDate();
		void printCouponLeg();

	private:

		cashflowLeg* _couponLeg;
		date _firstCouponDate;
		Market _market;
		std::string _name;
		std::string _type;
		std::string _shortCut;
		std::string _collateralType;
		double _couponRate;
		int _couponFreq;
		int _tenorNumOfMonths;
		int _firstCouponIndex;
		double _dirtyPrice;
		double _cleanPrice;
		double _YTM;
		double _fractionFirstCouponAccrued;
		enums::DayCountEnum _dayCount;
	};

}
#endif