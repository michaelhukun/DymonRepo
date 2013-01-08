//created by Hu Kun 30 Nov 2012
#include "MarketData.h"
#include "Configuration.h"
#include "DiscountCurveBuilder.h"
#include "SwaptionVolCubeBuilder.h"

using namespace Markets;
bool MarketData::instanceFlag = false;
MarketData* MarketData::single = NULL;

MarketData* MarketData::getInstance()
{
	if(! instanceFlag)
	{
		single = new MarketData();
		instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}

MarketData::~MarketData() {
	delete single;
	instanceFlag = false;
}

void MarketData::buildAll(){
	buildDiscountCurve();
	buildSwaptionVolCube();
}

void MarketData::buildDiscountCurve(){
	cout << "\n******** Build Discount Curve ********\n" << endl;
	DiscountCurveBuilder* builder = new DiscountCurveBuilder();
	_discountCurve = builder->build(Configuration::getInstance());
	cout<<_discountCurve->toString()<<endl;
}

void MarketData::buildSwaptionVolCube(){
	cout << "\n******** Build Swaption Vol Cube ********\n" << endl;
	SwaptionVolCubeBuilder* builder = new SwaptionVolCubeBuilder();
	_swaptionVolCube = builder->build(Configuration::getInstance());
}
