//created by Hu Kun Dec 13

#ifndef CASHFLOWLEGBUILDER_H
#define CASHFLOWLEGBUILDER_H

#include "cashflow.h"
#include "Market.h"
#include "cashflowLeg.h"
#include "Enums.h"
#include "date.h"
#include "dateUtil.h"
#include <vector>
#include "DiscountCurve.h"
#include "AbstractInstrument.h"
#include "swap.h"
#include "bond.h"

using namespace utilities;
using namespace std;
using namespace enums;
using namespace instruments;

namespace instruments {
	class CashFlowLegBuilder{
		
	public:
		CashFlowLegBuilder(){};
		CashFlowLegBuilder(AbstractInstrument* instrument);
		~CashFlowLegBuilder(){};

		// Methods		
		vector<cashflow>* build();

		// Getters and Setters
		date getIssueDate(){ return _issueDate; }
		date getAccuralStartDate(){ return _accrualStartDate; }
		date getAccuralEndDate(){ return _accrualEndDate; }
		date getDeliveryDate(){ return _deliveryDate; }
		int getPaymentFreq(){ return _paymentFreq; }
		int getPaymentNumber(){ return _paymentNumber; }
		enum::DayCountEnum getDayCountEnum(){ return _dayCount; }
		enum::DayRollEnum getDayRollEnum(){ return _dayRoll; }
		Market getMarket(){ return _market; }
		int getBuildDirection(){ return _buildDirection; }
		enums::Instrument getInstrumentEnum(){ return _instrumentEnum; }
		vector<cashflow>* getCashFlowVector(){ return &_cashflowVector;	}
		
		void setIssueDate(date issueDate){ _issueDate = issueDate; }
		void setAccuralStartDate(date accrualStartDate){ _accrualStartDate = accrualStartDate; }
		void setAccuralEndDate(date accrualEndDate){ _accrualEndDate = accrualEndDate; }
		void setDeliveryDate(date deliveryDate){ _deliveryDate = deliveryDate; }
		void setPaymentNumber(int paymentNumber){ _paymentNumber = paymentNumber; }
		void setPaymentFreq(int paymentFreq){ _paymentFreq = paymentFreq; }
		void setDayCountEnum(enums::DayCountEnum dayCount){ _dayCount = dayCount; }
		void setDayRollEnum(enums::DayRollEnum dayRoll){ _dayRoll = dayRoll; }
		void setMarket(Market market){ _market = market; }
		void setBuildDirection(int buildDirection){ _buildDirection = buildDirection; }
		void setInstrumentEnum(enums::Instrument instrumentEnum){ _instrumentEnum = instrumentEnum; }

	private:
		date _issueDate;
		date _accrualStartDate;
		date _accrualEndDate;
		date _deliveryDate;
		int _paymentNumber;
		int _paymentFreq;
		enums::DayCountEnum _dayCount;
		enums::DayRollEnum _dayRoll;
		Market _market;
		int _buildDirection; // 1=build from issueDate towards maturityDate
		vector<cashflow> _cashflowVector;
		enums::Instrument _instrumentEnum;	
	};
}
#endif