#include "Libor.h"
#include "dateUtil.h"

using namespace instruments;

void Libor::deriveDates(){	
	if (_isOverNight){
		_startDate = _fixingDate;
		if (_dateUnit==dateUtil::ON){
			_spotDate = _startDate;
		}else if (_dateUnit==dateUtil::TN){
			_spotDate = dateUtil::getBizDateOffSet(_startDate,1 , _market.getCurrencyEnum());
		}
		_expiryDate = dateUtil::getBizDateOffSet(_spotDate,1 , _market.getCurrencyEnum());
	}else{
		_startDate = _fixingDate;
		_spotDate = dateUtil::getBizDateOffSet(_fixingDate,_market.getBusinessDaysAfterSpotCash(),_market.getCurrencyEnum());
		_expiryDate = dateUtil::getEndDate(_spotDate,_tenorNum, _market.getAccrualAdjustCashConvention(),_market.getCurrencyEnum(), _dateUnit);
	}
	_deliveryDate = dateUtil::dayRollAdjust(_expiryDate,_market.getDayRollCashConvention(), _market.getCurrencyEnum());
}