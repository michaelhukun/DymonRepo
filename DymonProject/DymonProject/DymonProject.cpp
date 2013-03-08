#include "stdafx.h"
#include "date.h"
#include "dateUtil.h"
#include "cashflow.h"
#include "RecordHelper.h"
#include "Market.h"
#include "Enums.h"
#include "marketdata.h"
#include "TestManager.h"

using namespace utilities;
using namespace std;
using namespace Session;
using namespace UnitTest;
using namespace Markets;

void LoadInitialData();

int _tmain(int argc, _TCHAR* argv[])
{
	TestManager tm = TestManager();
	tm.init(0);
	tm.start();

	//LoadInitialData();
	//MarketData::getInstance()->buildAll();
}		

void LoadInitialData(){
	RecordHelper* recordHelper = RecordHelper::getInstance();
	Configuration* cfg = Configuration::getInstance();
	cfg->setFileName("instance.properties");
	recordHelper->init(cfg);
}