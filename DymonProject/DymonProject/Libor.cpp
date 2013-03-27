#include "Libor.h"
#include "dateUtil.h"

using namespace instruments;

void Libor::deriveDates(){	
	_startDate = dateUtil::getBizDateOffSet(_fixingDate,_market.getBusinessDaysAfterSpotCash(),_market.getCurrencyEnum());
	_spotDate = _startDate;
	_expiryDate = dateUtil::getEndDate(_startDate,_tenorInMonth, _market.getAccrualAdjustCashConvention(),_market.getCurrencyEnum(), dateUtil::MONTH);
	_deliveryDate = dateUtil::dayRollAdjust(_expiryDate,_market.getDayRollCashConvention(), _market.getCurrencyEnum());
}