//created by Wang Jianwei on 01 Dec 2012
#ifndef BONDPRICEFILESOURCE_H
#define BONDPRICEFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"
#include "Bond.h"

using namespace utilities;

namespace DAO {
	class BondPriceFileSource :	public AbstractFileSource{
	
	public:
		BondPriceFileSource():AbstractFileSource(){};
		BondPriceFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~BondPriceFileSource(){};

		void init(Configuration*);

		void retrieveRecord();

	private:

		void insertBondIntoCache(Bond* bond, RecordHelper::BondRateMap* bondRateMap);

		Bond* createBondObject(CSVDatabase db, int row);

		void updateMarketObjectField(std::string fieldName, std::string fieldVal, Bond* bond);
	};
}
#endif
