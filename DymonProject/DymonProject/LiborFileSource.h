//created by Wang Jianwei on 01 Dec 2012
#ifndef LIBORFILESOURCE_H
#define LIBORFILESOURCE_H
#include <string>
#include <fstream>
#include "AbstractFileSource.h"
#include <set>
#include <vector>
#include "Market.h"
#include "RecordHelper.h"
#include "Libor.h"
#include <map>

using namespace instruments;
using namespace utilities;

namespace DAO {
	class LiborFileSource: public AbstractFileSource{
		
	public:
		LiborFileSource(){};
		LiborFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){}
		~LiborFileSource(){};

		void init(Configuration*);
		
		void retrieveRecord();

	private:

		void insertLiborIntoCache(Libor* libor, RecordHelper::DepositRateMap* liborRateMap);

		Libor* createLiborObject(CSVDatabase db, int row);

		void updateLiborObjectField(std::string fieldName, std::string fieldVal, Libor* libor);

		void deriveTenor(std::string tenorStr, Libor* libor);
	};
}
#endif