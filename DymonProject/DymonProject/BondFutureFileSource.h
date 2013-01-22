//created by Wang Jianwei on 01 Dec 2012
#ifndef BONDFUTUREFILESOURCE_H
#define BONDFUTUREFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"
#include "BondFuture.h"
#include "Bond.h"

using namespace utilities;

namespace DAO {
	class BondFutureFileSource: public AbstractFileSource{
	
	public:
		BondFutureFileSource():AbstractFileSource(){};
		BondFutureFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~BondFutureFileSource(){};

		void init(Configuration*);

		void retrieveRecord();

	private:

		void insertBondFutureIntoCache(BondFuture* bondFuture, RecordHelper::BondFutureMap* bondFutureMap);

		BondFuture* createBondFutureObject(CSVDatabase db, int row);

		void updateBondFutureField(std::string fieldName, std::string fieldVal, BondFuture* tempBondFuture);
	};
}
#endif
