//created by Wang Jianwei on 01 Dec 2012
//added csv reading snippets on 13 Jan 2013-Kun
#ifndef ABSTRACTDATASOURCE_H
#define ABSTRACTDATASOURCE_H
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "AbstractDAO.h"

using namespace utilities;

namespace DAO {
	class  AbstractFileSource: public AbstractDAO{

	public:
		
		typedef std::string String;
		typedef std::vector<String> CSVRow;
		typedef CSVRow::const_iterator CSVRowCI;
		typedef std::vector<CSVRow> CSVDatabase;
		typedef CSVDatabase::const_iterator CSVDatabaseCI;

		// Constructors
		AbstractFileSource(){};
		AbstractFileSource(std::string persistDir, std::string fileName){
			_fileName = fileName;
			_persistDir = persistDir;
		}
		~AbstractFileSource(){};

		// Methods
		virtual void init(Configuration* cfg){
			AbstractDAO::init(cfg);
		};

		virtual void retrieveRecord()=0;

		// Getters and Setters
		std::string getFileName(){ return _fileName; }
		void setFileName(std::string fileName){ _fileName = fileName; }

	protected:

		CSVDatabase readCSV(std::string fileName);
		
		char* readText(std::string fileName);

		map<string, string> readMap(std::string fileName);

		void display(const CSVRow& row);

		void display(const CSVDatabase& db);
		
		std::string _fileName;
		std::string _persistDir;
	};
}
#endif