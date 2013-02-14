//created by Wang Jianwei on 01 Dec 2012
#ifndef ENUMHELPER_H
#define ENUMHELPER_H
#include "Enums.h"
#include <string>

using namespace enums;
using namespace std;

namespace utilities{
	class EnumHelper{

	public:

		EnumHelper(){};
		
		static enums::CurrencyEnum getCcyEnum(std::string ccyName);

		static enums::DayRollEnum getDayRollEnum(std::string dayRoll);

		static enums::DayCountEnum getDayCountEnum(std::string dayCount);

		static enums::interpolAlgo getInterpolAlgo(std::string interpolAlgo);

		static enums::NumericAlgo getNumericalAlgo(std::string numericalAlgo);

		static enums::OptionType getOptionType(std::string optionType);
	};
}
#endif