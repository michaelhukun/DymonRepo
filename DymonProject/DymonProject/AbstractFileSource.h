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

		AbstractFileSource(){};
		AbstractFileSource(std::string persistDir, std::string fileName);
		~AbstractFileSource(){};

		virtual void init(Configuration* cfg){
			AbstractDAO::init(cfg);
		};

		virtual char* readRecord();

		virtual void retrieveRecord();
		
		virtual void writeRecord(char* content);

		virtual void appendRecord(char* content);
		
		std::string getFileName();
		virtual void closeDataSource();
		virtual void deleteDataSource();		

	private:			

		std::ofstream _outFile;		
		long _fileSize;
		bool isModified;

	protected:

		void readCSV(std::ifstream &input, CSVDatabase &db);

		void display(const CSVRow& row);

		void display(const CSVDatabase& db);
		
		std::string _fileName;
		std::string _persistDir;
		char* _journal;
		std::ifstream _inFile;
		bool _enabled;
	};
}
#endif