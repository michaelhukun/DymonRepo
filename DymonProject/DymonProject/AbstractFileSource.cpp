//created by Wang Jianwei on 1 Dec 2012

#include "AbstractFileSource.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace DAO;
using namespace std;

AbstractFileSource::AbstractFileSource(std::string persistDir, std::string fileName){
	_fileName = fileName;
	_persistDir = persistDir;
	retrieveRecord();
}

char* AbstractFileSource::readRecord(){
	if (isModified){
		_inFile.open(_fileName);
		_inFile.seekg(0, ios::end);
		_fileSize = (long) _inFile.tellg();
		_inFile.seekg(0, ios::beg);
				
		if (_fileSize<0)
			throw "File Not Found: "+_fileName;

		_journal = new char[_fileSize];
		if (_inFile.is_open()){
			_inFile.read(_journal, _fileSize);
			cout<<"File read: "<<_journal<<endl;
		}else{
			throw "Cannot open input file\n";
		}
		isModified = false;
		_inFile.clear();
		_inFile.seekg(0, ios::beg);
	}
	return _journal;
}

void AbstractFileSource::retrieveRecord(){
	readRecord();
}

void AbstractFileSource::writeRecord(char* content){
	_outFile.open(_fileName);
	if (_outFile.is_open()){
		_outFile.write(content, sizeof(content));
		_outFile.close();
		isModified =true;
	}else{
		throw "Cannot open input file\n";
	}
}

void AbstractFileSource::appendRecord(char* content){
	_outFile.open(_fileName, ios::out | ios::app);
	if (_outFile.is_open()){
		_outFile.write(content, sizeof(content));
		_outFile.close();
		isModified =true;
	}else{
		throw "Cannot open input file\n";
	}
}

std::string AbstractFileSource::getFileName(){
	return _fileName;
}

void AbstractFileSource::closeDataSource(){
	_inFile.close();	
	_outFile.close();
}

void AbstractFileSource::deleteDataSource(){
	if( remove( _fileName.c_str() ) != 0 )
		throw "Error deleting file: "+_fileName+"\n";
	else
		cout<<"File successfully deleted: "<<_fileName<<endl;

}

void AbstractFileSource::readCSV(std::ifstream &input, CSVDatabase &db) {
	String csvLine;
	// read every line from the stream
	while( std::getline(input, csvLine) ){
		std::istringstream csvStream(csvLine);
		CSVRow csvRow;
		String csvCol;
		// read every element from the line that is seperated by commas
		// and put it into the vector or strings
		while( std::getline(csvStream, csvCol, ','))
			csvRow.push_back(csvCol);
		db.push_back(csvRow);
	}

};

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
		std::cout<<std::endl;
	}	
};




