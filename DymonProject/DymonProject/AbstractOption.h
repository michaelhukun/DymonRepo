//created by Jianwei on 30 Nov 2012
#ifndef ABSTRACTOPTION_H
#define ABSTRACTOPTION_H
#include "AbstractInstrument.h"
#include "OptionPricer.h"
#include "Enums.h"
#include "Market.h"
#include "DiscountCurve.h"

using namespace utilities;
using namespace instruments;

namespace instruments {
	class AbstractOption: public AbstractInstrument{

	public:

		enum DeltaType{BS, PREMIUM, FWDBS, FWDPREMIUM};

		AbstractOption(){};
		~AbstractOption(){};
		AbstractOption(Market market, date tradeDate, int expiryInMonth, VolType volType, double S, double K, double vol){
			BaseOption(tradeDate, volType, S, K, vol);
			setExpiryDate(dateUtil::getEndDate(tradeDate,expiryInMonth, enums::Null,market.getCurrencyEnum(),dateUtil::MONTH));
			setDeliveryDate(dateUtil::getEndDate(tradeDate,expiryInMonth, enums::Mfollowing,market.getCurrencyEnum(),dateUtil::MONTH));			
			_expiryInMonth = expiryInMonth;
			_market = market;
		}

		AbstractOption(date tradeDate, date expiryDate, date deliveryDate, VolType volType, double S, double K, double vol) {
			BaseOption(tradeDate, volType, S, K, vol);
			setExpiryDate(expiryDate);
			setDeliveryDate(deliveryDate);		
		}

		virtual double getMPV()=0;

		//Getters and Setters
		double getVol(){return _vol;}
		double getPrice(){ return _S; }
		double getStrike(){ return _K; }
		VolType getVolType(){ return _volType; }
		double getDiscountRate(){ return _r; }
		string getTenorStr(){ return _tenorStr; }
		DeltaType getDeltaType(){ return _deltaType; }

		void setVol(double vol){ _vol = vol; }
		void setPrice(double S){ _S=S; }
		void setStrike(double K){ _K=K; }
		void setTenorStr(string tenorStr){ _tenorStr=tenorStr; }
		void setVolType(VolType volType){ _volType = volType;}
		void setExpiryInMonth(int expiryInMonth){ _expiryInMonth = expiryInMonth;}
		void setDeltaType(DeltaType deltaType){ _deltaType = deltaType; }
		void setDiscountRate(double r){
			double accrualFactor = dateUtil::getAccrualFactor(_tradeDate, _expiryDate,enums::DayCountNull);
			_discountFactor = exp(-r*accrualFactor);
			_r=r;
		}
		void setDiscountCurve(DiscountCurve* discountCurve){
			_discountCurve = discountCurve;
			_discountFactor = discountCurve->getDiscountFactor(_tradeDate,_expiryDate);
			_r = discountCurve->getZeroRate(_tradeDate,_expiryDate, enums::DayCountNull);
		}

	protected:
		double _S;
		double _K;
		double _vol;
		double _r;
		double _discountFactor;
		double _expiryInMonth;
		DiscountCurve* _discountCurve;
		VolType _volType;
		std::string _tenorStr;
		DeltaType _deltaType;

		void BaseOption(date tradeDate, VolType volType, double S, double K, double vol) {
			setTradeDate(tradeDate);
			_volType = volType;
			_S = S;
			_K = K;
			_vol = vol;
		}
	};
}
#endif