//created by Hu Kun on 30 Nov
//updated by Hu Kun on 5 Dec

#include "Market.h"
#include "Enums.h"
#include "EnumHelper.h"
#include "RecordHelper.h"

using namespace instruments;
using namespace utilities;
using namespace Session;

Market::Market(enums::MarketEnum market) {
	_marketName = market;
	RecordHelper::MarketMap ccyMap = RecordHelper::getInstance()->getMarketMap();
	RecordHelper::MarketTuple ccyTuple =  ccyMap[market];
	setDayCountCashConvention(std::get<0>(ccyTuple));
	setDayCountSwapConvention(std::get<1>(ccyTuple));
	setDayRollCashConvention(std::get<2>(ccyTuple));
	setDayRollSwapConvention(std::get<3>(ccyTuple));
	setAccrualAdjustCashConvention(std::get<4>(ccyTuple));
	setAccrualAdjustSwapConvention(std::get<5>(ccyTuple));
	setBusinessDaysAfterSpot(std::get<6>(ccyTuple));
}

std::string Market::getNameString(){
	switch(_marketName){
	case::USD:
		return "USD";
	case::EUR:
		return "EUR";
	case::THB:
		return "THB";
	case::KRW:
		return "KRW";
	case::SGD:
		return "SGD";
	case::JPY:
		return "JPY";
	}
	return NULL;
}