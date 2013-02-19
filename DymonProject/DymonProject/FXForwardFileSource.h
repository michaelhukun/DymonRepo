//created by Wang Jianwei on 01 Dec 2012
#ifndef FXFORWARDFILESOURCE_H
#define FXFORWARDFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"
#include "FXForward.h"

using namespace utilities;

namespace DAO {
	class FXForwardFileSource :	public AbstractFileSource{
	
	public:
		FXForwardFileSource():AbstractFileSource(){};
		FXForwardFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~FXForwardFileSource(){};

		void init(Configuration*);

		void retrieveRecord();

	private:

		void insertForwardIntoCache(FXForward* forward, RecordHelper::FXForwardMap* FXForwardMap);

		FXForward* createForwardObject(CSVDatabase db, int row);

		void updateObjectField(std::string fieldName, std::string fieldVal, FXForward* forward);
	};
}
#endif