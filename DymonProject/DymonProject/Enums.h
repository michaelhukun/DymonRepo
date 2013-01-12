//created by Wang Jianwei on 30 Nov 2012

#ifndef _Market_H
#define _Market_H
namespace enums {
		
	enum MarketEnum{EUR,USD,THB,KRW,SGD,JPY};
	enum DayRollEnum {Null,Following,Preceding,Mfollowing,Mfollowingbi,EOM};
	enum DayCountEnum {thirty_360US, thirthE_360,ACT_360,ACT_365,ACT_ACT,BUS_252};
	enum NumericAlgo {BISECTION, RIDDER, SECANT, FALSEPOSITION};
	enum interpolAlgo{LINEAR,LOGLINEAR,CUBIC};
	enum CallPut{Call, Put};
	enum PayReceive {Payer, Receiver};
}

#endif