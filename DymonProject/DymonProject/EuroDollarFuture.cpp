#include "EuroDollarFuture.h"
#include "Libor.h"

using namespace instruments;

void EuroDollarFuture::genereateReset(){
	Libor* libor = new Libor();
	libor->setStartDate(_startDate);
	libor->setSpotDate(_startDate);
	libor->setTenorInMonth(_tenorInMonths);
	libor->setMarket(EUR);
	libor->setDayCount(enums::ACT_360);
	libor->deriveDates();
	setReset(*libor);
}

date EuroDollarFuture::getMaxFutureAndResetDeliveryDate(){
	date lastResetDeliveryDate = _reset.getDeliveryDate();
	if (_deliveryDate<lastResetDeliveryDate)
		return lastResetDeliveryDate;
	return _deliveryDate;
}