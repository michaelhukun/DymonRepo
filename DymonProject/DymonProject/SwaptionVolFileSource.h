//created by Hu Kun 30 Dec
#ifndef SWAPTIONVOLFILESOURCE_H
#define SWAPTIONVOLFILESOURCE_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "AbstractFileSource.h"
#include "RecordHelper.h"

namespace DAO {

	class SwaptionVolFileSource: public AbstractFileSource {

	public:

		SwaptionVolFileSource():AbstractFileSource(){};
		SwaptionVolFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~SwaptionVolFileSource(){};

		void init(Configuration*);

		void retrieveRecord();
		void swaptionTest();

	private:
	
		int getStrikeDiffATM(std::string strikeStr);

		void insertPointVolSurfaceMap(RecordHelper::SwaptionSurfaceMap &map, int fSwapTenorInMonth, int optionExpiryInMonth, double vol);
	
	};
}

#endif

