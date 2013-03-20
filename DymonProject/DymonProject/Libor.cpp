#include "Libor.h"
#include "dateUtil.h"

using namespace instruments;

void Libor::deriveDates(){
	_fixingDate = dateUtil::getBizDateOffSet(_startDate,-_market.getBusinessDaysAfterSpotCash(),_market.getCurrencyEnum());
	_expiryDate = dateUtil::getEndDate(_startDate,_tenorInMonth, _market.getAccrualAdjustCashConvention(),_market.getCurrencyEnum(), dateUtil::MONTH);
	_deliveryDate = dateUtil::dayRollAdjust(_expiryDate,_market.getDayRollCashConvention(), _market.getCurrencyEnum());
}