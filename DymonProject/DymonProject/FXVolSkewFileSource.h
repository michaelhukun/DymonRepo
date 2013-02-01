//created by Wang Jianwei on 01 Dec 2012
#ifndef FXVOLSKEWFILESOURCE_H
#define FXVOLSKEWFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"

using namespace utilities;

namespace DAO {
	class FXVolSkewFileSource: public AbstractFileSource{
	
	public:
		FXVolSkewFileSource():AbstractFileSource(){};
		FXVolSkewFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~FXVolSkewFileSource(){};

		void init(Configuration*);

		void retrieveRecord();

	private:

		void parseRow(std::string securityID, double vol);

		void insertFXVolIntoCache(CurrencyEnum CurrencyEnum, int expiry, int delta, double vol, RecordHelper::FXVolSkewMap* FXVolSkewMap);

	};
}
#endif
