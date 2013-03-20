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
		enums::DayCountEnum getDayCountSwapFixed(){return _dayCountSwapFixed;}
		enums::DayCountEnum getDayCountSwapFloat(){return _dayCountSwapFloat;}
		enums::DayCountEnum getDayCountBondConvention(){return _dayCountBondConvention;}
		enums::DayRollEnum getDayRollCashConvention(){return _dayRollCashConvention;}
		enums::DayRollEnum getDayRollSwapConvention(){return _dayRollSwapConvention;}
		enums::DayRollEnum getDayRollBondConvention(){return _dayRollBondConvention;}
		enums::DayRollEnum getAccrualAdjustCashConvention(){return _accrualAdjustCashConvention;}
		enums::DayRollEnum getAccrualAdjustSwapConvention(){return _accrualAdjustSwapConvention;}
		enums::DayRollEnum getAccrualAdjustBondConvention(){return _accrualAdjustBondConvention;}
		int getBusinessDaysAfterSpotSwap(){return _businessDaysAfterSpotSwap;}
		int getBusinessDaysAfterSpotBond(){return _businessDaysAfterSpotBond;}
		int getBusinessDaysAfterSpotCash(){return _businessDaysAfterSpotCash;}
		int getPayFreqSwapFixed(){ return _payFreqSwapFixed; }
		int getPayFreqSwapFloat(){ return _payFreqSwapFloat; }
		bool getShortEndUseLibor(){ return _shortEndUseLibor; }

		void setCurrencyEnum(enums::CurrencyEnum marketName){_marketName = marketName;}
		void setDayCountCashConvention(enums::DayCountEnum dayCountCashConvention){_dayCountCashConvention = dayCountCashConvention;}
		void setDayCountSwapFixed(enums::DayCountEnum dayCountSwapFixed){_dayCountSwapFixed = dayCountSwapFixed;}
		void setDayCountSwapFloat(enums::DayCountEnum dayCountSwapFloat){_dayCountSwapFloat = dayCountSwapFloat;}
		void setDayCountBondConvention(enums::DayCountEnum dayCountBondConvention){_dayCountBondConvention = dayCountBondConvention;}
		void setDayRollCashConvention(enums::DayRollEnum dayRollCashConvention){_dayRollCashConvention = dayRollCashConvention;}
		void setDayRollSwapConvention(enums::DayRollEnum dayRollSwapConvention){_dayRollSwapConvention = dayRollSwapConvention;}
		void setDayRollBondConvention(enums::DayRollEnum dayRollBondConvention){_dayRollBondConvention = dayRollBondConvention;}
		void setAccrualAdjustCashConvention(enums::DayRollEnum accrualAdjustCashConvention){_accrualAdjustCashConvention = accrualAdjustCashConvention;}
		void setAccrualAdjustSwapConvention(enums::DayRollEnum accrualAdjustSwapConvention){_accrualAdjustSwapConvention = accrualAdjustSwapConvention;}
		void setAccrualAdjustBondConvention(enums::DayRollEnum accrualAdjustBondConvention){_accrualAdjustBondConvention = accrualAdjustBondConvention;}
		void setBusinessDaysAfterSpotSwap(int businessDaysAfterSpotSwap){_businessDaysAfterSpotSwap =businessDaysAfterSpotSwap;}
		void setBusinessDaysAfterSpotBond(int businessDaysAfterSpotBond){_businessDaysAfterSpotBond =businessDaysAfterSpotBond;}
		void setBusinessDaysAfterSpotCash(int businessDaysAfterSpotCash){_businessDaysAfterSpotCash =businessDaysAfterSpotCash;}
		void setPayFreqSwapFixed(int payFreqSwapFixed){ _payFreqSwapFixed = payFreqSwapFixed; }
		void setPayFreqSwapFloat(int payFreqSwapFloat){ _payFreqSwapFloat = payFreqSwapFloat; }
		void setShortEndUseLibor(bool shortEndUseLibor){ _shortEndUseLibor = shortEndUseLibor; } 

		enums::DayRollEnum getDayRollConvention(enums::Instrument instrument);
		enums::DayRollEnum getAccrualAdjustConvention(enums::Instrument instrument);
		int getBusinessDaysAfterSpot(enums::Instrument instrument);

	private:
		enums::CurrencyEnum _marketName;
		enums::DayCountEnum _dayCountCashConvention;
		enums::DayCountEnum _dayCountSwapFixed;
		enums::DayCountEnum _dayCountSwapFloat;
		enums::DayCountEnum _dayCountBondConvention;
		enums::DayRollEnum _dayRollCashConvention;
		enums::DayRollEnum _dayRollSwapConvention;
		enums::DayRollEnum _dayRollBondConvention;
		enums::DayRollEnum _accrualAdjustCashConvention;
		enums::DayRollEnum _accrualAdjustSwapConvention;
		enums::DayRollEnum _accrualAdjustBondConvention;
		int _businessDaysAfterSpotSwap;
		int _businessDaysAfterSpotBond;
		int _businessDaysAfterSpotCash;
		int _payFreqSwapFixed;
		int _payFreqSwapFloat;
		bool _shortEndUseLibor;
	};
}
#endif