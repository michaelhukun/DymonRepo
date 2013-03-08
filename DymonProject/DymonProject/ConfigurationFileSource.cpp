//created by Wang Jianwei on 1 Dec 2012

#include "ConfigurationFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "Configuration.h"

using namespace DAO;
using namespace std;
using namespace Session;
using namespace utilities;

void ConfigurationFileSource::init(Configuration* cfg){
	_name = "Configuration";
	_fileName = cfg->getFileName();
	_persistDir = "";
	AbstractFileSource::init(cfg);
}

void ConfigurationFileSource::retrieveRecord(){
	map<string, string> configMap = readMap(_fileName);
	Configuration::getInstance()->setConfiguration(configMap);
}
