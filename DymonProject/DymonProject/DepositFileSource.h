//created by Wang Jianwei on 01 Dec 2012
#ifndef DEPOSITFILESOURCE_H
#define DEPOSITFILESOURCE_H
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
	class DepositFileSource: public AbstractFileSource{
		
	public:
		DepositFileSource();
		DepositFileSource(std::string persistDir, std::string fileName);
		~DepositFileSource();

		void init(Configuration*);
		
		void retrieveRecord();

	private:

		void insertDepositIntoCache(Deposit* deposit, RecordHelper::DepositRateMap* depositRateMap);

		Deposit* createDepositObject(CSVDatabase db, int row);

		void updateDepositObjectField(std::string fieldName, std::string fieldVal, Deposit* deposit);

	};
}
#endif