#include "BondPriceFileSource.h"

using namespace DAO;
void BondPriceFileSource::init(Configuration* cfg){
	_fileName = cfg->getProperty("bondPrice.file",true,"");
	_persistDir = cfg->getProperty("bondPrice.path",false,"");
	AbstractFileSource::init(cfg);
}

void BondPriceFileSource::retrieveRecord(){
}
