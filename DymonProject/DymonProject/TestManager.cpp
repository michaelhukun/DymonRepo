#include "TestManager.h"
#include "RecordHelper.h"
#include "marketdata.h"
#include "TestGenericSwap.h"
#include "TestGenericBond.h"
#include "TestDateUtil.h"

using namespace UnitTest;
using namespace utilities;
using namespace Markets;

void TestManager::init(Configuration*){
	RecordHelper* recordHelper = RecordHelper::getInstance();
	_cfg = Configuration::getInstance();
	_cfg->setFileName("test.properties");
	recordHelper->init(_cfg);
}

void TestManager::start(){
	MarketData::getInstance()->buildAll();
	_testVector.push_back(new TestDateUtil());
	_testVector.push_back(new TestGenericSwap());
	_testVector.push_back(new TestGenericBond());

	for(unsigned int i = 0; i<_testVector.size(); i++){
		AbstractTest* test = _testVector[i];
		test->init(_cfg);
		test->runTest();
	}
}

void TestManager::stop(){
}