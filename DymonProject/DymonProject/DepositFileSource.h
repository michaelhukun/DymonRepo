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
		void insertRateIntoMap(std::string tenorStr, double liborRate, Market market, std::map<long, double>* depositRateMap, std::map<long, double>* overnightRateMap);

	};

}
#endif