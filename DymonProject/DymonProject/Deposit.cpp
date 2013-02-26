#include "Deposit.h"

using namespace instruments;

void Deposit::deriveAccrualStartDate(){
	if (_isOverNight){
		_spotDate = _tradeDate;
	}else{
		_spotDate = date(_expiryDate - _daysToMty);
	}
}