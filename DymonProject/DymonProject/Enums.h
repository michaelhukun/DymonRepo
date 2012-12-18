//created by Wang Jianwei on 30 Nov 2012

#ifndef _CURRENCY_H
#define _CURRENCY_H
namespace enums {
		
	enum CurrencyEnum{EUR,USD,THB,KRW,SGD,JPY};
	enum DayRollEnum {NullDayRoll,Following,Preceding,Mfollowing,Mfollowingbi,EOM};
	enum DayCountEnum {NullDayCount,thirty_360US, thirthE_360,ACT_360,ACT_365,ACT_ACT,BUS_252};
	enum NumericAlgo {BISECTION, RIDDER, SECANT, FALSEPOSITION};
	enum interpolAlgo{LINEAR,LOGLINEAR};
}

#endif