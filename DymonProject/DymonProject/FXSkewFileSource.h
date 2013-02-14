//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWFILESOURCE_H
#define FXSKEWFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"
#include "Enums.h"

using namespace utilities;

namespace DAO {
	class FXSkewFileSource: public AbstractFileSource{
	
	public:
		FXSkewFileSource():AbstractFileSource(){};
		FXSkewFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~FXSkewFileSource(){};

		void init(Configuration*);

		void retrieveRecord();

	private:

		void parseRow(std::string securityID, double vol, RecordHelper::FXVolSkewMap* FXVolSkewMap);

		int getTenorIndex(std::string deltaTenorStr);

		int getDelta(std::string deltaStr);

		int getDaysToExpiry(std::string deltaTenorStr, int tenorIndex, enums::CurrencyEnum market);

		void insertFXVolIntoCache(std::string currencyPairStr, int daysToExpiry, int delta, enums::OptionType optionType, double vol, RecordHelper::FXVolSkewMap* FXVolSkewMap);

	};
}
#endif
