#include "Deposit.h"

using namespace instruments;

void Deposit::deriveAccrualStartDate(){
		_spotDate = date(_expiryDate - _daysToMty);
}