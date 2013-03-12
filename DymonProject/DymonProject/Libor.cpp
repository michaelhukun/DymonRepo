#include "Libor.h"
#include "dateUtil.h"

using namespace instruments;

void Libor::deriveAccrualStartDate(){
	if (_isOverNight){
		_spotDate = _tradeDate;
	}else{
		_spotDate = dateUtil::getBizDateOffSet(_tradeDate,_market.getBusinessDaysAfterSpotSwap(),_market.getCurrencyEnum());
	}
}