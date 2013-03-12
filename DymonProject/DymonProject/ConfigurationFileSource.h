//created by Wang Jianwei on 01 Dec 2012
#ifndef CONFIGURATIONFILESOUTCE_H
#define CONFIGURATIONFILESOUTCE_H
#include <string>
#include <fstream>
#include "AbstractFileSource.h"

using namespace utilities;

namespace DAO {
	class ConfigurationFileSource: public AbstractFileSource{
		
	public:
		ConfigurationFileSource(){};
		ConfigurationFileSource(std::string persistDir, std::string fileName):AbstractFileSource(persistDir, fileName){}
		~ConfigurationFileSource(){};

		void init(Configuration*);
		
		void retrieveRecord();
	};

}
#endif