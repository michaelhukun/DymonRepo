//created by Hu Kun on 30 Nov 2012
//class for standard IRS with fix to floating legs in one Market.
//rates for fixed and floating in private data members are all in terms of annual rates
//rewrote again with cashflow constructs by Kun 16 Dec 2012
//revamped by Jianwei on 23/2/2012

#ifndef SWAP_H
#define SWAP_H
#include "Market.h"
#include "date.h"
#include "AbstractInstrument.h"
#include "cashflow.h"
#include "cashflowLeg.h"
#include "CashFlowLegBuilder.h"
#include "DiscountCurve.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

namespace instruments {
	class Swap: public AbstractInstrument{
	public:
		Swap():AbstractInstrument(){
			_fixedCashflowLeg = cashflowLeg();
			_floatCashflowLeg = cashflowLeg();
			_instrumentEnum = enums::SWAP;
		};
		~Swap(){};

		// Getters and Setters

		int getPayFreqFixed() { return _fixedCashflowLeg.getCouponFreq(); }
		int getPayFreqFloat() { return _floatCashflowLeg.getCouponFreq(); }
		cashflowLeg* getCashFlowLegFix() { return &_fixedCashflowLeg; }
		cashflowLeg* getCashFlowLegFloat() {	return &_floatCashflowLeg; }
		DiscountCurve* Swap::getDiscountCurve() { return _yc; }
		int getTenor(){ return _tenorInYear;}
		double getSwapRate(){ return _swapRate; }
		double getDaysToMty(){ return _daysToMty; }
		enums::DayCountEnum getDayCountFixed(){ return _fixedCashflowLeg.getDayCount(); }
		enums::DayCountEnum getDayCountFloat(){ return _floatCashflowLeg.getDayCount(); }
		int getCouponNumberFixed(){ return _fixedCashflowLeg.getCashFlowNumber(); }
		int getCouponNumberFloat(){ return _floatCashflowLeg.getCashFlowNumber(); }

		void setSwapRate(double swapRate){ _swapRate= swapRate; }
		void setDaysToMty(int daysToMty){ _daysToMty = daysToMty; }
		void setDayCountFixed(enums::DayCountEnum dayCountFixed){ _fixedCashflowLeg.setDayCount(dayCountFixed);}
		void setDayCountFloat(enums::DayCountEnum dayCountFloat){ _floatCashflowLeg.setDayCount(dayCountFloat);}
		void setPayFreqFixed(int payFreqFixed){ _fixedCashflowLeg.setCouponFreq(payFreqFixed);}
		void setPayFreqFloat(int payFreqFloat){ _floatCashflowLeg.setCouponFreq(payFreqFloat);}
		void setTenorInYear(int tenorInYear){ _tenorInYear = tenorInYear; }


		// Methods
		void deriveDates();
		void deriveDayCount();
		void buildFixedLeg();
		void buildFloatLeg();
		void printCashflowLegFix();
		void printCashflowLegFloat();
		std::string toString(){return "";}

	private:

		cashflowLeg _fixedCashflowLeg;
		cashflowLeg _floatCashflowLeg;
		DiscountCurve* _yc;
		int _tenorInYear;
		double _swapRate;
		int _daysToMty;
	};

}
#endif