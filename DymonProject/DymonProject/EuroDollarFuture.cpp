#include "EuroDollarFuture.h"
#include "Libor.h"
#include "dateUtil.h"

using namespace instruments;
using namespace utilities;

void EuroDollarFuture::genereateReset(){
	Libor* libor = new Libor();
	libor->setTenorInMonth(_tenorInMonths);
		libor->setMarket(Market(GBP));
	libor->setDayCount(enums::ACT_360);
	libor->setFixingDate(dateUtil::getBizDateOffSet(_startDate, -_market.getBusinessDaysAfterSpotCash(),_market.getCurrencyEnum()));
	libor->deriveDates();
	setReset(*libor);
}

date EuroDollarFuture::getMaxFutureAndResetDeliveryDate(){
	date lastResetDeliveryDate = _reset.getDeliveryDate();
	//if (_deliveryDate<lastResetDeliveryDate)
		return lastResetDeliveryDate;
	//return _deliveryDate;
}