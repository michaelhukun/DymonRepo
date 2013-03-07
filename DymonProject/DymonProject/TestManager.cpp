#include "TestManager.h"
#include "RecordHelper.h"

using namespace UnitTest;
using namespace utilities;

void TestManager::init(Configuration*){
	RecordHelper* recordHelper = RecordHelper::getInstance();
	Configuration* cfg = Configuration::getInstance();
	cfg->setFileName("test.properties");
	recordHelper->init(cfg);
}

void TestManager::start(){
}

void TestManager::stop(){
}