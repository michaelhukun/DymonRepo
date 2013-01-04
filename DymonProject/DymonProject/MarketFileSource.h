//created by Wang Jianwei on 01 Dec 2012
#ifndef MarketFILESOURCE_H
#define MarketFILESOURCE_H
#include <string>
#include <fstream>
#include "AbstractFileSource.h"
#include <set>
#include <tuple>

namespace DAO {
	class MarketFileSource: public AbstractFileSource{
		
	public:
		
		MarketFileSource();
		MarketFileSource(std::string persistDir, std::string fileName);
		~MarketFileSource();

		void init(Configuration*);
		
		void retrieveRecord();
	};
}
#endif