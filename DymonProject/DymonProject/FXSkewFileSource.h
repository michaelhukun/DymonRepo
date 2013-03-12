//created by Wang Jianwei on 01 Dec 2012
#ifndef FXSKEWFILESOURCE_H
#define FXSKEWFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"
#include "Enums.h"
#include "FXEuropeanOption.h"

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

		void insertOptionIntoCache(FXEuropeanOption* option, RecordHelper::FXVolSkewMap* FXVolSkewMap);

		FXEuropeanOption* createOptionObject(CSVDatabase db, int row);

		void updateOptionObjectField(std::string fieldName, std::string fieldVal, FXEuropeanOption* option);

	};
}
#endif
