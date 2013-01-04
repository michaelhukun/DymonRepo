#include "SwaptionVolCubeBuilder.h"
#include "EnumHelper.h"

using namespace utilities;
typedef AbstractBuilder super;

void SwaptionVolCubeBuilder::init(Configuration* cfg){
	super::init(cfg);
	
	_market = Market(EnumHelper::getCcyEnum("USD"));
	_interpolAlgo = EnumHelper::getInterpolAlgo(cfg->getProperty("swaptionVolCube.usd.interpol",false,"LINEAR"));
}

SwaptionVolCube* SwaptionVolCubeBuilder::build(){
	SwaptionVolCube* svc = new SwaptionVolCube();
	return svc;
}