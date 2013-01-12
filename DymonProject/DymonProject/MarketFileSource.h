//created by Wang Jianwei on 01 Dec 2012
#ifndef MarketFILESOURCE_H
#define MarketFILESOURCE_H
#include <string>
#include <fstream>
#include "AbstractFileSource.h"
#include <set>
#include <tuple>
#include "Market.h"

using namespace instruments;

namespace DAO {
	class MarketFileSource: public AbstractFileSource{
		
	public:
		
		MarketFileSource():AbstractFileSource(){};
		MarketFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~MarketFileSource(){};

		void updateMarketObjectField(std::string fieldName, std::string fieldVal, Market* market);

		void init(Configuration*);
		
		void retrieveRecord();
	};
}
#endif