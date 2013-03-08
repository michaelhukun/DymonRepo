//created by Wang Jianwei on 1 Dec 2012

#include "HolidayFileSource.h"
#include "AbstractFileSource.h"
#include "fileUtil.h"
#include "dateUtil.h"
#include "date.h"
#include "RecordHelper.h"
#include "Enums.h"
#include "EnumHelper.h"

using namespace DAO;
using namespace std;
using namespace utilities;
using namespace Session;

void HolidayFileSource::init(Configuration* cfg){
   _name = "Holiday";
	_fileName = cfg->getProperty("holiday.file",true,"");
	_persistDir = cfg->getProperty("data.path",false,"");
	_enabled = cfg->getProperty("holiday.enabled",true,"")=="true"?true:false;
	AbstractFileSource::init(cfg);
}

void HolidayFileSource::retrieveRecord(){
	if (!_enabled) return;

	std::ifstream file;
	file.open(_persistDir+_fileName);
	string value;
	enums::CurrencyEnum market;
	RecordHelper::HolidayMap tempMap;

	while (file.good()){
		file>>value;
		vector<string> vec = fileUtil::split(value,':');
		market = EnumHelper::getCcyEnum(vec[0]);
		vector<string> holidays = fileUtil::split(vec[1],',');
		//cout<<market<<" market has total holiday number:  "<<holidays.size()<<endl;
		set<long> JDNSet = buildJDNSet(holidays);
		tempMap.insert(pair<enums::CurrencyEnum,set<long>>(market,JDNSet));
	}
	RecordHelper::getInstance()->setHolidayMap(tempMap);
	file.close();
}

set<long> HolidayFileSource::buildJDNSet(vector<string> vec0){
	set<long> JDNSet;
	for(unsigned int i=0; i<vec0.size(); i++) {
		long JDN = dateUtil::getJudianDayNumber(stoi(vec0[i].substr(0,4)),stoi(vec0[i].substr(4,2)),stoi(vec0[i].substr(6,2)));
		JDNSet.insert(JDN);
	}
	return JDNSet;
}
