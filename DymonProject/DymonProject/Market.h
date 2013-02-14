//created by Hu Kun on 29 Nov 2012

#ifndef Market_H
#define Market_H
#include <tuple>
#include "dateUtil.h"
#include "Enums.h"

namespace instruments {

	class Market {

	public:
		Market(){};
		Market(enums::CurrencyEnum);
		~Market(){};

		std::string getNameString();
		void display();

		enums::CurrencyEnum getCurrencyEnum(){return _marketName;}
		enums::DayCountEnum getDayCountCashConvention(){return _dayCountCashConvention;}
		enums::DayCountEnum getDayCountSwapConvention(){return _dayCountSwapConvention;}
		enums::DayCountEnum getDayCountBondConvention(){return _dayCountBondConvention;}
		enums::DayRollEnum getDayRollCashConvention(){return _dayRollCashConvention;}
		enums::DayRollEnum getDayRollSwapConvention(){return _dayRollSwapConvention;}
		enums::DayRollEnum getDayRollBondConvention(){return _dayRollBondConvention;}
		enums::DayRollEnum getAccrualAdjustCashConvention(){return _accrualAdjustCashConvention;}
		enums::DayRollEnum getAccrualAdjustSwapConvention(){return _accrualAdjustSwapConvention;}
		enums::DayRollEnum getAccrualAdjustBondConvention(){return _accrualAdjustBondConvention;}
		int getBusinessDaysAfterSpotSwap(){return _businessDaysAfterSpotSwap;}
		int getBusinessDaysAfterSpotBond(){return _businessDaysAfterSpotBond;}
		int getBusinessDaysAfterExpiryFXOption(){return _businessDaysAfterExpiryFXOption;}

		void setCurrencyEnum(enums::CurrencyEnum marketName){_marketName = marketName;}
		void setDayCountCashConvention(enums::DayCountEnum dayCountCashConvention){_dayCountCashConvention = dayCountCashConvention;}
		void setDayCountSwapConvention(enums::DayCountEnum dayCountSwapConvention){_dayCountSwapConvention = dayCountSwapConvention;}
		void setDayCountBondConvention(enums::DayCountEnum dayCountBondConvention){_dayCountBondConvention = dayCountBondConvention;}
		void setDayRollCashConvention(enums::DayRollEnum dayRollCashConvention){_dayRollCashConvention = dayRollCashConvention;}
		void setDayRollSwapConvention(enums::DayRollEnum dayRollSwapConvention){_dayRollSwapConvention = dayRollSwapConvention;}
		void setDayRollBondConvention(enums::DayRollEnum dayRollBondConvention){_dayRollBondConvention = dayRollBondConvention;}
		void setAccrualAdjustCashConvention(enums::DayRollEnum accrualAdjustCashConvention){_accrualAdjustCashConvention = accrualAdjustCashConvention;}
		void setAccrualAdjustSwapConvention(enums::DayRollEnum accrualAdjustSwapConvention){_accrualAdjustSwapConvention = accrualAdjustSwapConvention;}
		void setAccrualAdjustBondConvention(enums::DayRollEnum accrualAdjustBondConvention){_accrualAdjustBondConvention = accrualAdjustBondConvention;}
		void setBusinessDaysAfterSpotSwap(int businessDaysAfterSpotSwap){_businessDaysAfterSpotSwap =businessDaysAfterSpotSwap;}
		void setBusinessDaysAfterSpotBond(int businessDaysAfterSpotBond){_businessDaysAfterSpotBond =businessDaysAfterSpotBond;}
		void setBusinessDaysAfterExpiryFXOption(int businessDaysAfterExpiryFXOption){_businessDaysAfterExpiryFXOption =businessDaysAfterExpiryFXOption;}
		
		enums::DayCountEnum getDayCountConvention(enums::Instrument instrument);
		enums::DayRollEnum getDayRollConvention(enums::Instrument instrument);
		enums::DayRollEnum getAccrualAdjustConvention(enums::Instrument instrument);
		int getBusinessDaysAfterSpot(enums::Instrument instrument);
		int getBusinessDaysAfterExpiry(enums::Instrument instrument);

	private:
		enums::CurrencyEnum _marketName;
		enums::DayCountEnum _dayCountCashConvention;
		enums::DayCountEnum _dayCountSwapConvention;
		enums::DayCountEnum _dayCountBondConvention;
		enums::DayRollEnum _dayRollCashConvention;
		enums::DayRollEnum _dayRollSwapConvention;
		enums::DayRollEnum _dayRollBondConvention;
		enums::DayRollEnum _accrualAdjustCashConvention;
		enums::DayRollEnum _accrualAdjustSwapConvention;
		enums::DayRollEnum _accrualAdjustBondConvention;
		int _businessDaysAfterSpotSwap;
		int _businessDaysAfterSpotBond;
		int _businessDaysAfterExpiryFXOption;
	};

}
#endif