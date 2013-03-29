#include "EuroDollarFuture.h"
#include "Libor.h"
#include "dateUtil.h"

using namespace instruments;
using namespace utilities;

void EuroDollarFuture::genereateReset(){
	Libor* libor = new Libor();
	libor->setTenorNum(_tenorInMonths);
	libor->setDateUnit(dateUtil::MONTH);
	libor->setMarket(Market(GBP));
	libor->setDayCount(enums::ACT_360);
	libor->setIsOverNight(false);
	libor->setFixingDate(_deliveryDate);
	libor->deriveDates();
	setReset(*libor);
}

date EuroDollarFuture::getResetDeliveryDate(){
	date lastResetDeliveryDate = _reset.getDeliveryDate();
	//if (_deliveryDate<lastResetDeliveryDate)
	return lastResetDeliveryDate;
	//return _deliveryDate;
}