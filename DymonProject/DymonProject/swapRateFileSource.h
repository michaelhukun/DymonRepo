//created by Wang Jianwei on 01 Dec 2012
#ifndef SWAPRATEFILESOURCE_H
#define SWAPRATEFILESOURCE_H
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
	class SwapRateFileSource: public AbstractFileSource{
		
	public:
		SwapRateFileSource();
		SwapRateFileSource(std::string persistDir, std::string fileName);
		~SwapRateFileSource();

		void init(Configuration*);
		
		void retrieveRecord();

	private:
		
		void insertSwapIntoCache(Swap* option, RecordHelper::SwapRateMap* swapRateMap);

		Swap* createSwapObject(CSVDatabase db, int row);

		void updateSwapObjectField(std::string fieldName, std::string fieldVal, Swap* swap);

	};

}
#endif