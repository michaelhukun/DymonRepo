//created by Hu Kun on 8 Dec
//modified by Hu Kun on 10 Dec
//reviewed and modified by Hu Kun on 11 Dec, added running trade features

#include "cashflow.h"
#include <math.h>
#include <stdio.h>
#include "date.h"
#include "dateUtil.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

cashflow::cashflow(double couponRate,double notional,  date fixingDate, date paymentDate,date accuralStartDate, date accuralEndDate, Market cashFlowCurr, bool isValid) {
	setCouponRate(couponRate);
	setNotional(notional);
	setFixingDate(fixingDate);
	setPaymentDate(paymentDate);
	setAccuralStartDate(accuralStartDate);
	setAccuralEndDate(accuralEndDate);
	setCashFlowCurr(cashFlowCurr);
	deriveAccuralFactor();
	setIsValid(isValid);
}

cashflow::cashflow(Deposit* deposit, bool isValid){
	setCouponRate(deposit->getDepositRate());
	setNotional(0);
	setFixingDate(deposit->getTradeDate());
	setPaymentDate(deposit->getDeliveryDate());
	setAccuralStartDate(deposit->getSpotDate());
	setAccuralEndDate(deposit->getExpiryDate());
	setCashFlowCurr(deposit->getMarket());
	setDayCount(deposit->getDayCount());
	deriveAccuralFactor();
	setIsValid(isValid);
}

double cashflow::getCouponRate() {
	return _couponRate;
}

date cashflow::getFixingDate() {
	return _fixingDate;
}

date cashflow::getPaymentDate() {
	return _paymentDate;
}

date cashflow::getAccuralStartDate() {
	return _accuralStartDate;
}

date cashflow::getAccuralEndDate() {
	return _accuralEndDate;
}

Market cashflow::getCashFlowCurr() {
	return _cashFlowCurr;
}

double cashflow::getAccuralFactor() {
	return _accuralFactor;
}

double cashflow::getNotional() {
	return _notional;
}

void cashflow::setCouponRate(double couponRate) {
	_couponRate=couponRate;
}
void cashflow::setNotional(double notional) {
	_notional=notional;
}

void cashflow::setFixingDate(date fixingDate) {
	_fixingDate=fixingDate;
}

void cashflow::setPaymentDate(date paymentDate) {
	_paymentDate=paymentDate;
}

void cashflow::setAccuralStartDate(date accuralStartDate) {
	_accuralStartDate=accuralStartDate;
}

void cashflow::setAccuralEndDate(date accuralEndDate) {
	_accuralEndDate=accuralEndDate;
}

void cashflow::setCashFlowCurr(Market cashFlowCurr) {
	_cashFlowCurr=cashFlowCurr;
}

void cashflow::deriveAccuralFactor() {
	_accuralFactor=dateUtil::getAccrualFactor(_accuralStartDate,_accuralEndDate,_dayCount);
}

bool cashflow::getIsValid(){
	return _isValid;
}

void cashflow::setIsValid(bool isValid){
	_isValid = isValid;
}

bool cashflow::isDateEqual(cashflow cf){
	if (!getAccuralEndDate().isEqual(cf.getAccuralEndDate())) return false;
	if (!getAccuralStartDate().isEqual(cf.getAccuralStartDate())) return false;
	if (!getFixingDate().isEqual(cf.getFixingDate())) return false;
	if (!getPaymentDate().isEqual(cf.getPaymentDate())) return false;
	return true;
}

void cashflow::printCashFlow() {
	cout<<"fixingDate ["<< _fixingDate.toString()<<"], accuralStartDate ["<< _accuralStartDate.toString()<<"], "<<
		"accuralEndDate ["<<_accuralEndDate.toString()<<"] paymentDate["<< _paymentDate.toString()<<"]"<<endl;
	 cout<<"accuralFactor ["<<_accuralFactor<<"], couponRate["<<_couponRate<<"], notional["<<_notional<<"]"<<endl;
}
