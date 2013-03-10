//created by Wang Jianwei on 01 Dec 2012
#ifndef CCYPAIRFILESOURCE_H
#define CCYPAIRFILESOURCE_H
#include "AbstractFileSource.h"
#include "RecordHelper.h"
#include "CcyPair.h"

using namespace instruments;
using namespace utilities;

namespace DAO {
	class CcyPairFileSource: public AbstractFileSource{
		
	public:
		CcyPairFileSource(){};
		CcyPairFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){}
		~CcyPairFileSource(){};

		void init(Configuration* cfg);
		
		void retrieveRecord();

	private:
		
		void insertCcyPairIntoCache(CcyPair* ccypair, RecordHelper::CcyPairMap* CcyPairMap);

		CcyPair* createCcyPairObject(CSVDatabase db, int row);

		void updateCcyPairObjectField(std::string fieldName, std::string fieldVal, CcyPair* ccypair);

	};

}
#endif