//Written by Jianwei
#ifndef CCYPAIR_H
#define CCYPAIR_H
#include <string>
#include "Enums.h"

using namespace std;

namespace utilities {
	class CcyPair {

	public:
		CcyPair(std::string ccyPairStr){
			setCcyPairStr(ccyPairStr);
		}
		CcyPair(){}
		~CcyPair(){}

		//Getters and Setters
		void setCCY1(enums::CurrencyEnum ccy1){_ccy1 = ccy1;}
		void setCCY2(enums::CurrencyEnum ccy2){_ccy2 = ccy2;}
		void setCcyPairStr(std::string ccyPairStr);
		enums::CurrencyEnum getCCY1(){return _ccy1;}
		enums::CurrencyEnum getCCY2(){return _ccy2;}
		enums::CurrencyEnum getDomesticCCY(){return _ccy1;}
		enums::CurrencyEnum getForeignCCY(){return _ccy2;}
		std::string getCcyPairStr(){return _ccyPairStr;}
		bool isEmergingMarket(){return _isEmergingMarket;}

		// Operator overload
		bool operator != (CcyPair ccyPair){ !isEqual(ccyPair);}
		bool operator == (CcyPair ccyPair){ isEqual(ccyPair);}

		// Methods
		bool isEqual(CcyPair ccyPair);
		std::string toString(){return _ccyPairStr;}

	private:

		enums::CurrencyEnum _ccy1;
		enums::CurrencyEnum _ccy2;
		std::string _ccyPairStr;
		bool _isEmergingMarket;
	};
}
#endif