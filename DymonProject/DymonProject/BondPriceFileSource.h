//created by Wang Jianwei on 01 Dec 2012
#ifndef BONDPRICEFILESOURCE_H
#define BONDPRICEFILESOURCE_H
#include "abstractfilesource.h"

namespace DAO {
	class BondPriceFileSource :	public AbstractFileSource{
	
	public:
		BondPriceFileSource():AbstractFileSource(){};
		BondPriceFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~BondPriceFileSource(){};

		void init(Configuration*);

		void retrieveRecord();
	};
}
#endif
