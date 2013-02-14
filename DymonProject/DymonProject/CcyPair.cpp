#include "CcyPair.h"
#include <regex>
#include "EnumHelper.h"

using namespace utilities;
using namespace std;

void CcyPair::setCcyPairStr(std::string ccyPairStr){
	std::regex ccyPairRegex ("[A-Za-z]{6}");
	if (std::regex_match (ccyPairStr,ccyPairRegex)){
		string ccy1 = ccyPairStr.substr(0,3);
		string ccy2 = ccyPairStr.substr(3,3);
		setCCY1(EnumHelper::getCcyEnum(ccy1));
		setCCY2(EnumHelper::getCcyEnum(ccy2));
	}else{
		throw "currency pair not recognized!";
	}
	_ccyPairStr = ccyPairStr;
}

bool CcyPair::isEqual(CcyPair ccyPair){
	if (getCCY1() == ccyPair.getCCY1() && getCCY2() == ccyPair.getCCY2())
		return true;
	else
		return false;
}