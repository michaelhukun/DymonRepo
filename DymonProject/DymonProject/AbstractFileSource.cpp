//created by Wang Jianwei on 1 Dec 2012

#include "AbstractFileSource.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace DAO;
using namespace std;

AbstractFileSource::CSVDatabase AbstractFileSource::readCSV(std::string fileName) {
	CSVDatabase db;
	String csvLine;
	std::ifstream file;
	file.open(fileName);
	// read every line from the stream
	while( std::getline(file, csvLine) ){
		std::istringstream csvStream(csvLine);
		CSVRow csvRow;
		String csvCol;
		// read every element from the line that is seperated by commas
		// and put it into the vector or strings
		while( std::getline(csvStream, csvCol, ','))
			csvRow.push_back(csvCol);
		db.push_back(csvRow);
	}
	file.close();
	return db; 
};


char* AbstractFileSource::readText(std::string fileName){
	std::ifstream file;
	file.open(fileName);
	file.seekg(0, ios::end);
	int fileSize = (long) file.tellg();
	file.seekg(0, ios::beg);

	if (file<0)
		throw "File Not Found: "+_fileName;

	char* journal = new char[fileSize];
	if (file.is_open()){
		file.read(journal, fileSize);
		//cout<<"File read: "<<_journal<<endl;
	}else{
		throw "Cannot open input file\n";
	}
	file.close();

	return journal;
}


map<string, string> AbstractFileSource::readMap(std::string fileName){	
	std::ifstream file;
	file.open(fileName);
	string keyEqualsValue;
	map<string, string> configMap;
	while (file.good()){
		file>>keyEqualsValue;
		if (keyEqualsValue.find('=')==0)
		{
			cout<<"Property ignored - Equal sign not found in: "<<keyEqualsValue<<endl;
			continue;
		}
		string key = keyEqualsValue.substr(0,keyEqualsValue.find('='));
		string value = keyEqualsValue.substr(keyEqualsValue.find('=')+1);
		if (value==""||key=="")
		{
			cout<<"Property ignored - Key/value pair not complete: "<<keyEqualsValue<<endl;
			continue;
		}
		configMap[key]=value;
		//cout << "Item \"" << key << "\" has value \"" << value << '\"' << endl ;
	}	
	file.close();
	return configMap;
}

void AbstractFileSource::display(const CSVRow& row) {
	if(!row.size())
		return;
	CSVRowCI i=row.begin();
	std::cout<<*(i++);
	for(;i != row.end();++i)
		std::cout<<','<<*i;
};

void AbstractFileSource::display(const CSVDatabase& db) {
	if(!db.size())
		return;
	CSVDatabaseCI i=db.begin();
	for(; i != db.end(); ++i){
		display(*i);
	}	
};




