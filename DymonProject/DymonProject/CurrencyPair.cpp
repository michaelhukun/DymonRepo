#include "CurrencyPair.h"
#include <regex>
#include "EnumHelper.h"

using namespace utilities;

CurrencyPair::CurrencyPair(std::string currencyPair){
	std::regex ccypairRegex ("[A-Za-z]{6}");
	if (std::regex_match (currencyPair,ccypairRegex)){
		string ccy1 = currencyPair.substr(0,3);
		string ccy2 = currencyPair.substr(3,3);
		setCCY1(EnumHelper::getCcyEnum(ccy1));
		setCCY2(EnumHelper::getCcyEnum(ccy2));
	}else{
		throw "Currency Pair not recognized!";
	}
}

bool CurrencyPair::isEqual(CurrencyPair ccyPair){
	if (getCCY1() == ccyPair.getCCY1() && getCCY2() == ccyPair.getCCY2())
		return true;
	else
		return false;
}