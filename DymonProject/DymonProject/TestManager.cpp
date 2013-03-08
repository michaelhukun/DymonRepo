#include "TestManager.h"
#include "RecordHelper.h"
#include "marketdata.h"

using namespace UnitTest;
using namespace utilities;
using namespace Markets;

void TestManager::init(Configuration*){
	RecordHelper* recordHelper = RecordHelper::getInstance();
	Configuration* cfg = Configuration::getInstance();
	cfg->setFileName("test.properties");
	recordHelper->init(cfg);
}

void TestManager::start(){
	MarketData::getInstance()->buildAll();


}

void TestManager::stop(){
}