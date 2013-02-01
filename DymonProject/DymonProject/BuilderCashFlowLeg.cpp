//created by Hu Kun Dec 13
#include "cashflowLeg.h"
#include "cashflow.h"
#include "BuilderCashFlowLeg.h"
#include <math.h>
#include <stdio.h>
#include "date.h"
#include "dateUtil.h"
#include <vector>
#include "RecordHelper.h"
#include <algorithm>
#include "DiscountCurve.h"
#include "Constants.h"
#include "Market.h"

using namespace Session;
using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

BuilderCashFlowLeg::BuilderCashFlowLeg(enums::Instrument instrument, date issueDate, date maturityDate, int tenorNumOfMonths, double couponRate, double notional, int paymentFreq, enums::CurrencyEnum market, int buildDirection){
	
	Market mkt(market);
	enums::DayRollEnum dayRollConvention = mkt.getDayRollConvention(instrument);
	enums::DayRollEnum accrualAdjustConvention = mkt.getAccrualAdjustConvention(instrument);
	date accrualStartDate = dateUtil::getBizDateOffSet(issueDate,mkt.getBusinessDaysAfterSpot(instrument),market);
	int numOfMonthIncr=12/(paymentFreq==NaN?1:paymentFreq);
	int totalPaymentNum = paymentFreq==NaN?1:(int) (tenorNumOfMonths/12.0*paymentFreq);
	int i=0;
	vector<cashflow> cashflowVector;

	if (buildDirection==1) {
		while(i<=totalPaymentNum-1){
			date calDateNewStart=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
			date calDateNewEnd=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);			
			date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
			date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

			cashflow aCashflow(couponRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
			cashflowVector.push_back(aCashflow);
			i++;
		}

		// Adjust final cash flow in case its accrual end date missmatch with maturity
		cashflow* lastCashFlow = &cashflowVector[cashflowVector.size()-1];
		if (lastCashFlow->getAccuralEndDate()!=maturityDate){
			lastCashFlow->setAccuralEndDate(maturityDate);
			lastCashFlow->setPaymentDate(dateUtil::dayRollAdjust(maturityDate,dayRollConvention,market));
		}
	}

	if (buildDirection==-1) {
 		while(i<=totalPaymentNum-1){

			date calDateNewStart=dateUtil::getEndDate(maturityDate,-numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);
			date calDateNewEnd=dateUtil::getEndDate(maturityDate,-numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
			date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
			date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

			cashflow aCashflow(couponRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
			cashflowVector.insert(cashflowVector.begin(),aCashflow);
			i++;
		}
		
		// Adjust final cash flow in case its accrual end date missmatch with maturity
		cashflow* firstCashFlow = &cashflowVector[0];
		if (firstCashFlow->getAccuralStartDate()!=issueDate){
			firstCashFlow->setAccuralStartDate(issueDate);
			firstCashFlow->setFixingDate(dateUtil::getBizDateOffSet(issueDate,-mkt.getBusinessDaysAfterSpot(instrument),market));
		}
	}

	_cashflowLeg = new cashflowLeg(cashflowVector, paymentFreq);
}

BuilderCashFlowLeg::BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, int tenorNumOfMonths,double couponRate,double notional, int paymentFreq, enums::CurrencyEnum market){

	Market mkt(market);
	enums::DayRollEnum dayRollConvention = mkt.getDayRollConvention(instrument);
	enums::DayRollEnum accrualAdjustConvention = mkt.getAccrualAdjustConvention(instrument);
	accrualStartDate = dateUtil::getBizDateOffSet(accrualStartDate,mkt.getBusinessDaysAfterSpot(instrument),market);

	int numOfMonthIncr=12/paymentFreq;
	int i=0;
	vector<cashflow> cashflowVector;

	while((numOfMonthIncr*i<tenorNumOfMonths)&&(numOfMonthIncr*(i+1)<=tenorNumOfMonths)){

		date calDateNewStart=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
		date calDateNewEnd=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);			
		date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
		date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

		cashflow aCashflow(couponRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
		cashflowVector.push_back(aCashflow);
		i++;
	}

	if ((numOfMonthIncr*(i-1)>tenorNumOfMonths)||(numOfMonthIncr*(i)>tenorNumOfMonths)) {

		date calDateNewStart=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
		date calDateNewEnd=dateUtil::getEndDate(accrualStartDate,tenorNumOfMonths,accrualAdjustConvention,market,dateUtil::MONTH);

		date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
		date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);
		cashflow aCashflow(couponRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);

		cashflowVector.push_back(aCashflow);
	}
	_cashflowLeg = new cashflowLeg(cashflowVector, paymentFreq);
}

BuilderCashFlowLeg::BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, date accrualEndDate, int tenorNumOfMonths, DiscountCurve* yc,double notional, int paymentFreq, enums::CurrencyEnum market, int buildDirection){

	Market mkt(market);
	enums::DayRollEnum dayRollConvention = mkt.getDayRollConvention(instrument);
	enums::DayRollEnum accrualAdjustConvention = mkt.getAccrualAdjustConvention(instrument);
	accrualStartDate = dateUtil::getBizDateOffSet(accrualStartDate,mkt.getBusinessDaysAfterSpot(instrument),market);
	int numOfMonthIncr=12/(paymentFreq==NaN?1:paymentFreq);
	int i=0;
	vector<cashflow> cashflowVector;

	if (buildDirection==1) {
		while(dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH)<=accrualEndDate){

			date calDateNewStart=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
			date calDateNewEnd=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);			
			date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
			date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

			double FLiborRate=yc->getFLiborRate(calDateNewStart,calDateNewEnd,mkt.getDayCountSwapConvention());
			cashflow aCashflow(FLiborRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
			cashflowVector.push_back(aCashflow);
			i++;
		}

		if (dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH)!=accrualEndDate)
			throw "Derived end date is not the same as indicated.";
	}

	if (buildDirection==-1) {			
		while(dateUtil::getEndDate(accrualEndDate,-numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH)>=accrualStartDate){

			date calDateNewStart=dateUtil::getEndDate(accrualEndDate,-numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);
			date calDateNewEnd=dateUtil::getEndDate(accrualEndDate,-numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
			date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
			date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

			double FLiborRate=yc->getFLiborRate(calDateNewStart,calDateNewEnd,mkt.getDayCountSwapConvention());
			cashflow aCashflow(FLiborRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
			cashflowVector.insert(cashflowVector.begin(),aCashflow);
			i++;
		}

		if (dateUtil::getEndDate(accrualEndDate,-numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH)>accrualStartDate) {

			date calDateNewStart=dateUtil::getEndDate(accrualEndDate,-numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);
			//date calDateNewStart=dateUtil::dayRollAdjust(accrualStartDate,accrualAdjustConvention,market);
			date calDateNewEnd=dateUtil::getEndDate(accrualEndDate,-numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
			date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
			date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

			double FLiborRate=yc->getFLiborRate(calDateNewStart,calDateNewEnd,mkt.getDayCountSwapConvention());
			cashflow aCashflow(FLiborRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
			cashflowVector.insert(cashflowVector.begin(),aCashflow);
		}
	}
	_cashflowLeg = new cashflowLeg(cashflowVector, paymentFreq);
}

BuilderCashFlowLeg::BuilderCashFlowLeg(enums::Instrument instrument, date accrualStartDate, int tenorNumOfMonths,DiscountCurve* yc,double notional, int paymentFreq, enums::CurrencyEnum market){
	
	Market mkt(market);
	enums::DayRollEnum dayRollConvention = mkt.getDayRollConvention(instrument);
	enums::DayRollEnum accrualAdjustConvention = mkt.getAccrualAdjustConvention(instrument);
	accrualStartDate = dateUtil::getBizDateOffSet(accrualStartDate,mkt.getBusinessDaysAfterSpot(instrument),market);

	int numOfMonthIncr=12/paymentFreq;
	int i=0;
	vector<cashflow> cashflowVector;

	while((numOfMonthIncr*i<tenorNumOfMonths)&&(numOfMonthIncr*(i+1)<=tenorNumOfMonths)){

		date calDateNewStart=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
		date calDateNewEnd=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*(i+1),accrualAdjustConvention,market,dateUtil::MONTH);			
		date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
		date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

		double FLiborRate=yc->getFLiborRate(calDateNewStart,calDateNewEnd,mkt.getDayCountSwapConvention());
		cashflow aCashflow(FLiborRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
		cashflowVector.push_back(aCashflow);
		i++;
	}

	if ((numOfMonthIncr*(i-1)>tenorNumOfMonths)||(numOfMonthIncr*i>tenorNumOfMonths)) {

		date calDateNewStart=dateUtil::getEndDate(accrualStartDate,numOfMonthIncr*i,accrualAdjustConvention,market,dateUtil::MONTH);
		date calDateNewEnd=dateUtil::getEndDate(accrualStartDate,tenorNumOfMonths,accrualAdjustConvention,market,dateUtil::MONTH);
		date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-mkt.getBusinessDaysAfterSpot(instrument),market);
		date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,dayRollConvention,market);

		double FLiborRate=yc->getFLiborRate(calDateNewStart,calDateNewEnd,mkt.getDayCountSwapConvention());
		cashflow aCashflow(FLiborRate, notional, calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, market, true);
		cashflowVector.push_back(aCashflow);
	}
	_cashflowLeg = new cashflowLeg(cashflowVector, paymentFreq);
}
