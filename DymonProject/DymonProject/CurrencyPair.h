//Written by Jianwei
#ifndef CURRENCYPAIR_H
#define CURRENCYPAIR_H

#include <iostream>
#include <string>
#include "Enums.h"

using namespace std;

namespace utilities {
	class CurrencyPair {

	public:
		CurrencyPair(){}
		~CurrencyPair(){}
		CurrencyPair(std::string currencyPair);

		void setCCY1(enums::CurrencyEnum ccy1){_ccy1 = ccy1;}
		void setCCY2(enums::CurrencyEnum ccy2){_ccy2 = ccy2;}
		enums::CurrencyEnum getCCY1(){return _ccy1;}
		enums::CurrencyEnum getCCY2(){return _ccy2;}

		bool operator != (CurrencyPair ccyPair){ !isEqual(ccyPair);}
		bool operator == (CurrencyPair ccyPair){ isEqual(ccyPair);}
		bool isEqual(CurrencyPair ccyPair);

	private:

		enums::CurrencyEnum _ccy1;
		enums::CurrencyEnum _ccy2;
	};
}
#endif