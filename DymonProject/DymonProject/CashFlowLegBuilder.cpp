//created by Hu Kun Dec 13
#include "cashflowLeg.h"
#include "cashflow.h"
#include "CashFlowLegBuilder.h"
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

CashFlowLegBuilder::CashFlowLegBuilder(AbstractInstrument* instrument){
	setIssueDate(instrument->getIssueDate());
	setAccuralStartDate(instrument->getStartDate());
	setAccuralEndDate(instrument->getExpiryDate());
	setDeliveryDate(instrument->getDeliveryDate());
	setMarket(instrument->getMarket());
	setInstrumentEnum(instrument->getInstrumentEnum());
	setDayCountEnum(instrument->getDayCount());
	setDayRollEnum(instrument->getDayRoll());
	setNotional(instrument->getNotional());
}

vector<cashflow>* CashFlowLegBuilder::build(){
	_cashflowVector.clear();
   int numOfMonthIncr=12/_paymentFreq;
	enums::DayRollEnum accrualAdjust = _market.getAccrualAdjustConvention(_instrumentEnum);
	enums::CurrencyEnum marketEnum = _market.getCurrencyEnum();

	for(int i=0; i<_paymentNumber; i++){
      cashflow cf;
      if (_buildDirection==1) {
		  date calDateNewStart=dateUtil::getEndDate(_accrualStartDate,numOfMonthIncr*i,accrualAdjust, marketEnum, dateUtil::MONTH);
		  date calDateNewEnd=dateUtil::getEndDate(_accrualStartDate,numOfMonthIncr*(i+1),accrualAdjust,marketEnum, dateUtil::MONTH);			
		  date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-_market.getBusinessDaysAfterSpot(_instrumentEnum), marketEnum);
		  date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd, _dayRoll, marketEnum);
		  cf = cashflow(calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, _dayCount, marketEnum, true);
		  cf.setNotional(_notional);
		  _cashflowVector.push_back(cf);
	  }else if (_buildDirection==-1) {
		  date calDateNewStart=dateUtil::getEndDate(_accrualEndDate,-numOfMonthIncr*(i+1),accrualAdjust,marketEnum,dateUtil::MONTH);
		  date calDateNewEnd=dateUtil::getEndDate(_accrualEndDate,-numOfMonthIncr*i,accrualAdjust,marketEnum,dateUtil::MONTH);
		  date calFixingDate=dateUtil::getBizDateOffSet(calDateNewStart,-_market.getBusinessDaysAfterSpot(_instrumentEnum),marketEnum);
		  date calPaymentDate=dateUtil::dayRollAdjust(calDateNewEnd,_dayRoll, marketEnum);
		  cf = cashflow(calFixingDate, calPaymentDate, calDateNewStart, calDateNewEnd, _dayCount, marketEnum, true);
		  cf.setNotional(_notional);
		  _cashflowVector.insert(_cashflowVector.begin(),cf);
	  }
	}

	checkEndDate(numOfMonthIncr, accrualAdjust, marketEnum);
   return &_cashflowVector;
}

void CashFlowLegBuilder::checkEndDate(int numOfMonthIncr, enums::DayRollEnum accrualAdjust, enums::CurrencyEnum marketEnum){
   if (_buildDirection==1) {
	   cashflow* cf = &_cashflowVector.at(_cashflowVector.size()-1);
      if (cf->getAccuralEndDate()!=_accrualEndDate){
         if (_joinMismatchedEndPoint){
            cf->setAccuralEndDate(_accrualEndDate);
         }else{
            _cashflowVector.clear();
            throw "Derived end date is not the same as indicated.";
         }
      }
   }else if (_buildDirection==-1) {
      cashflow* cf = &_cashflowVector.at(0);
      if (cf->getAccuralStartDate()!=_accrualStartDate){
         if (_joinMismatchedEndPoint){
            cf->setAccuralStartDate(_accrualStartDate);
         }else{
            _cashflowVector.clear();
            throw "Derived start date is not the same as indicated.";
         }
      }
   }
}
