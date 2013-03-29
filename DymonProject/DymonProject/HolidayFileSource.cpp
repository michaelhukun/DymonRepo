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

	CSVDatabase db = readCSV(_persistDir+_fileName);
	int numOfRows=db.size();
	int numOfCols=db.at(0).size();

	for (int i=0;i<numOfCols;i++) {
		set<long> JDNSet;
		enums::CurrencyEnum market= EnumHelper::getCcyEnum(db.at(0).at(i));
		for(int j=1; j<numOfRows; j++){
			try{
				std::string dateStr =  db.at(j).at(i);
				if (dateStr=="") continue;
				long JDN = date(dateStr, false).getJudianDayNumber();
				JDNSet.insert(JDN);
			}catch (...){
			}
		}
		RecordHelper::getInstance()->getHolidayMap()->insert(pair<enums::CurrencyEnum,set<long>>(market,JDNSet));
	}
}