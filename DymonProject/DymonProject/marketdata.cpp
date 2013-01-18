//created by Hu Kun 30 Nov 2012
#include "MarketData.h"
#include "Configuration.h"
#include "SwapCurveBuilder.h"
#include "SwaptionVolCubeBuilder.h"
#include "BondCurveBuilder.h"

using namespace Markets;
using namespace utilities;
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
	buildSwapDiscountCurve();
	buildBondDiscountCurve();
	buildSwaptionVolCube();
}

void MarketData::buildSwapDiscountCurve(){
	cout << "\n******** Build Swap Discount Curve ********\n" << endl;
	SwapCurveBuilder* builder = new SwapCurveBuilder();
	_SwapDiscountCurve = builder->build(Configuration::getInstance());
	cout<<_SwapDiscountCurve->toString()<<endl;
}

void MarketData::buildBondDiscountCurve(){
	cout << "\n******** Build Bond Discount Curve ********\n" << endl;
	BondCurveBuilder* builder = new BondCurveBuilder();
	_BondDiscountCurve = builder->build(Configuration::getInstance());
	cout<<_BondDiscountCurve->toString()<<endl;
}

void MarketData::buildSwaptionVolCube(){
	cout << "\n******** Build Swaption Vol Cube ********\n" << endl;
	SwaptionVolCubeBuilder* builder = new SwaptionVolCubeBuilder();
	_swaptionVolCube = builder->build(Configuration::getInstance());
}
