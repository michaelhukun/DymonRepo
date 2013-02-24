//created by Wang Jianwei on 01 Dec 2012

#include "RecordHelper.h"
#include "AbstractDao.h"
#include "Configuration.h"
#include "swapRateFileSource.h"
#include "HolidayFileSource.h"
#include "DepositFileSource.h"
#include "ConfigurationFileSource.h"
#include "MarketFileSource.h"
#include "SwaptionVolFileSource.h"
#include "BondPriceFileSource.h"
#include "BondFutureFileSource.h"
#include "FXSkewFileSource.h"
#include "FXForwardFileSource.h"
#include <vector>

using namespace Session;
using namespace DAO;
using namespace std;

RecordHelper* RecordHelper::single = NULL;

RecordHelper* RecordHelper::getInstance()
{
	if(! single)
		single = new RecordHelper();
	return single;
}

void RecordHelper::init(Configuration* cfg){
	dataSourceVector.push_back(new ConfigurationFileSource());
	dataSourceVector.push_back(new MarketFileSource());
	dataSourceVector.push_back(new HolidayFileSource());
	dataSourceVector.push_back(new SwapRateFileSource());
	dataSourceVector.push_back(new DepositFileSource());
	dataSourceVector.push_back(new SwaptionVolFileSource());
	dataSourceVector.push_back(new BondPriceFileSource());
	dataSourceVector.push_back(new BondFutureFileSource());
	dataSourceVector.push_back(new FXForwardFileSource());
	dataSourceVector.push_back(new FXSkewFileSource());
	
	for(unsigned int i = 0; i<dataSourceVector.size(); i++){
		AbstractDAO* dao = dataSourceVector[i];
		dao->init(cfg);
		dao->retrieveRecord();
	}
}

Bond* RecordHelper::findCTDinBondMap(std::string CUSIP){
	BondRateMap::iterator bondMapIt;
	for (bondMapIt=_bondRateMap.begin(); bondMapIt!=_bondRateMap.end(); ++bondMapIt){
		map<long, Bond>* innerBondMap = &(bondMapIt->second);
		map<long, Bond>::iterator innerBondMapIt;
		for (innerBondMapIt=innerBondMap->begin(); innerBondMapIt!=innerBondMap->end(); ++innerBondMapIt){
			Bond* tempBond = &(innerBondMapIt->second);
			if (tempBond->getCUSIP() == CUSIP)
				return tempBond;
		}
	}
	return NULL;
}

date RecordHelper::getSpotDate(string ccyPairStr){
	for (auto it = _FXForwardMap.begin(); it!=_FXForwardMap.end(); it++){
		if (ccyPairStr == it->first){
			auto* innerMap = &(it->second);
			for (auto innerIt = innerMap->begin(); innerIt!=innerMap->end(); innerIt++){
				FXForward* forward = &(innerIt->second);
				if (forward->getIsSpot()){
					return forward->getSpotDate();
				}
			}
			throw "Spot Date not found!";
		}
	}
	throw "Currency pair not found!";
}