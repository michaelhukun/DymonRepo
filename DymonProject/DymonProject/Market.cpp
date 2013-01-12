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
	RecordHelper::MarketMap marketMap = RecordHelper::getInstance()->getMarketMap();
	*this = marketMap[market];
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

void Market::display(){
	
	cout << this->getNameString()<< " -> "<< "DayCountCashConvention "<< _dayCountCashConvention<<endl;
	cout << this->getNameString()<< " -> "<< "DayCountSwapConvention "<< _dayCountSwapConvention<<endl;
	cout << this->getNameString()<< " -> "<< "DayCountBondConvention "<< _dayCountBondConvention<<endl;
	cout << this->getNameString()<< " -> "<< "DayRollCashConvention "<< _dayRollCashConvention<<endl;
	cout << this->getNameString()<< " -> "<< "DayRollSwapConvention "<< _dayRollSwapConvention<<endl;
	cout << this->getNameString()<< " -> "<< "DayRollBondConvention "<< _dayRollBondConvention<<endl;
	cout << this->getNameString()<< " -> "<< "AccrualAdjustCashConvention "<< _accrualAdjustCashConvention<<endl;
	cout << this->getNameString()<< " -> "<< "AccrualAdjustSwapConvention "<< _accrualAdjustSwapConvention<<endl;
	cout << this->getNameString()<< " -> "<< "BusinessDaysAfterSpot "<< _businessDaysAfterSpot<<endl;
}