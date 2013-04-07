//created by Wang Jianwei on 01 Dec 2012
#ifndef EURODOLLARFUTUREFILESOURCE_H
#define EURODOLLARFUTUREFILESOURCE_H
#include <string>
#include <fstream>
#include "AbstractFileSource.h"
#include <set>
#include <vector>
#include "Market.h"
#include "RecordHelper.h"
#include <map>

using namespace instruments;
using namespace utilities;

namespace DAO {
	class EuroDollarFutureFileSource: public AbstractFileSource{
		
	public:
		EuroDollarFutureFileSource(){};
		EuroDollarFutureFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){}
		~EuroDollarFutureFileSource(){};

		void init(Configuration*);
		
		void retrieveRecord();

	private:

		void insertFutureIntoCache(EuroDollarFuture* deposit, RecordHelper::EuroDollarFutureMap* euroDollarFutureMap);

		EuroDollarFuture* createFutureObject(CSVDatabase db, int row);

		void updateFutureObjectField(std::string fieldName, std::string fieldVal, EuroDollarFuture* euroDollarFuture);

	};
}
#endif