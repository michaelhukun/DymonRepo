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

		void parseRow(std::string securityID, double vol);

		int getTenorIndex(std::string deltaTenorStr);

		double getDelta(std::string deltaStr);

		double getTenorExpiry(string tenorStr, char tenorUnit, enums::CurrencyEnum marketEnum);
		
		double getTenorDiscount(string tenorStr, char tenorUnit, enums::CurrencyEnum marketEnum);

		void insertFXVolIntoCache(std::string ccyPairStr, double tenorExpiry, double tenorDiscount, double delta, OptionType optionType, double vol);

	};
}
#endif
