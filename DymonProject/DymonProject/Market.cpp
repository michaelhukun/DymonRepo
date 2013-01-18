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

enums::DayCountEnum Market::getDayCountConvention(enums::Instrument instrument){
	switch (instrument){
	case enums::BOND:
		return getDayCountBondConvention();
	case enums::SWAP:
		return getDayCountSwapConvention();
	default:
		return getDayCountSwapConvention();
	}
}

enums::DayRollEnum Market::getDayRollConvention(enums::Instrument instrument){
	switch (instrument){
	case enums::BOND:
		return getDayRollBondConvention();
	case enums::SWAP:
		return getDayRollSwapConvention();
	default:
		return getDayRollSwapConvention();
	}
}

enums::DayRollEnum Market::getAccrualAdjustConvention(enums::Instrument instrument){
	switch (instrument){
	case enums::BOND:
		return getAccrualAdjustBondConvention();
	case enums::SWAP:
		return getAccrualAdjustSwapConvention();
	default:
		return getAccrualAdjustSwapConvention();
	}
}

int Market::getBusinessDaysAfterSpot(enums::Instrument instrument){
	switch (instrument){
	case enums::BOND:
		return getBusinessDaysAfterSpotBond();
	case enums::SWAP:
		return getBusinessDaysAfterSpotSwap();
	default:
		return getBusinessDaysAfterSpotSwap();
	}
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
	cout << this->getNameString()<< " -> "<< "AccrualAdjustBondConvention "<< _accrualAdjustBondConvention<<endl;
	cout << this->getNameString()<< " -> "<< "BusinessDaysAfterSpotSwap"<< _businessDaysAfterSpotSwap<<endl;
	cout << this->getNameString()<< " -> "<< "BusinessDaysAfterSpotBond"<< _businessDaysAfterSpotBond<<endl;
}