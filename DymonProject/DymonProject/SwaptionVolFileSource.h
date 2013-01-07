//created by Hu Kun 30 Dec
#ifndef SWAPTIONVOLFILESOURCE_H
#define SWAPTIONVOLFILESOURCE_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "AbstractFileSource.h"
#include "RecordHelper.h"

namespace DAO {

	class SwaptionVolFileSource: public AbstractFileSource {
		typedef std::string String;
		typedef std::vector<String> CSVRow;
		typedef CSVRow::const_iterator CSVRowCI;
		typedef std::vector<CSVRow> CSVDatabase;
		typedef CSVDatabase::const_iterator CSVDatabaseCI;

	public:

		SwaptionVolFileSource():AbstractFileSource(){};
		SwaptionVolFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){};
		~SwaptionVolFileSource(){};

		void init(Configuration*);

		void retrieveRecord();
		void swaptionTest();

	private:
		void readCSV(std::ifstream &input, CSVDatabase &db);

		void display(const CSVRow& row);

		void display(const CSVDatabase& db);

		int getStrikeDiffATM(std::string strikeStr);

		void insertPointVolSurfaceMap(RecordHelper::SwaptionSurfaceMap &map, int fSwapTenorInMonth, int optionExpiryInMonth, double vol);
	
	};
}

#endif

