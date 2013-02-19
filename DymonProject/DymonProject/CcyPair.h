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
		void setCCY1(std::string ccy1){_ccy1 = ccy1;}
		void setCCY2(std::string ccy2){_ccy2 = ccy2;}
		void setCcyPairStr(std::string ccyPairStr);
		std::string getCCY1(){return _ccy1;}
		std::string getCCY2(){return _ccy2;}

		enums::CurrencyEnum getCCY1Enum();
		enums::CurrencyEnum getCCY2Enum();
		std::string getDomesticCCY(){return _ccy1;}
		std::string getForeignCCY(){return _ccy2;}
		std::string getCcyPairStr(){return _ccy1+_ccy2;}
		bool isEmergingMarket(){return _isEmergingMarket;}

		// Operator overload
		bool operator == (CcyPair ccyPair){ isEqual(ccyPair);}

		// Methods
		bool isEqual(CcyPair ccyPair);
		std::string toString(){return _ccyPairStr;}

	private:

		std::string _ccy1;
		std::string _ccy2;
		std::string _ccyPairStr;
		bool _isEmergingMarket;
	};
}
#endif