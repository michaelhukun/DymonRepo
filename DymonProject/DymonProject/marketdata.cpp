//created by Hu Kun 30 Nov 2012
#include "MarketData.h"
#include "Configuration.h"
#include "SwapCurveBuilder.h"
#include "SwaptionVolCubeBuilder.h"
#include "FXSkewSurfaceBuilder.h"
#include "BondCurveBuilder.h"
#include "RecordHelper.h"

using namespace Markets;
using namespace utilities;
using namespace enums;
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
	buildFXSkewSurface();
}

void MarketData::buildSwapDiscountCurve(){
	cout << "\n******** Build Swap Discount Curve ********\n" << endl;
	RecordHelper::RateMap::iterator it;
	RecordHelper::RateMap rateMap = RecordHelper::getInstance()->getDepositRateMap();
	for (it = rateMap.begin(); it!= rateMap.end(); ++it){
		CurrencyEnum ccyEnum = it->first;
		SwapCurveBuilder* builder = new SwapCurveBuilder(ccyEnum);
		DiscountCurve* curve = builder->build(Configuration::getInstance());
		_SwapDiscountCurveMap.insert(pair<CurrencyEnum, DiscountCurve>(ccyEnum, *curve));
		cout<<curve->toString()<<endl;
	}
}

void MarketData::buildBondDiscountCurve(){
	cout << "\n******** Build Bond Discount Curve ********\n" << endl;
	RecordHelper::BondRateMap::iterator it;
	RecordHelper::BondRateMap* rateMap = RecordHelper::getInstance()->getBondRateMap();
	for (it = rateMap->begin(); it!= rateMap->end(); ++it){
		CurrencyEnum ccyEnum = it->first;
		BondCurveBuilder* builder = new BondCurveBuilder(ccyEnum);
		DiscountCurve* curve = builder->build(Configuration::getInstance());
		_BondDiscountCurveMap.insert(pair<CurrencyEnum, DiscountCurve>(ccyEnum, *curve));
		cout<<curve->toString()<<endl;
	}
}

void MarketData::buildSwaptionVolCube(){
	cout << "\n******** Build Swaption Vol Cube ********\n" << endl;
	SwaptionVolCubeBuilder* builder = new SwaptionVolCubeBuilder(USD);
	SwaptionVolCube* cube = builder->build(Configuration::getInstance());
	_SwaptionVolCubeMap.insert(pair<CurrencyEnum, SwaptionVolCube>(USD, *cube));
}

void MarketData::buildFXSkewSurface(){
	cout << "\n******** Build FX Skew Surface ********\n" << endl;
	RecordHelper::FXVolSkewMap::iterator it;
	RecordHelper::FXVolSkewMap* rateMap = RecordHelper::getInstance()->getFXVolSkewMap();
	for (it = rateMap->begin(); it!= rateMap->end(); ++it){
		CurrencyEnum ccyEnum = it->first;
		FXSkewSurfaceBuilder* builder = new FXSkewSurfaceBuilder(ccyEnum);
		FXSkewSurface* surface = builder->build(Configuration::getInstance());
		_FXSkewSurfaceMap.insert(pair<CurrencyEnum, FXSkewSurface>(ccyEnum, *surface));
		cout<<surface->toString()<<endl;
	}
}

DiscountCurve* MarketData::getSwapDiscountCurve(enums::CurrencyEnum market){
	if ( _SwapDiscountCurveMap.find(market) == _SwapDiscountCurveMap.end()) {
		throw "Swap curve not found in map!";
	}
	else {
		return &_SwapDiscountCurveMap.find(market)->second;
	}
}

DiscountCurve* MarketData::getBondDiscountCurve(enums::CurrencyEnum market){
	if ( _BondDiscountCurveMap.find(market) == _BondDiscountCurveMap.end()) {
		throw "Bond curve not found in map!";
	}
	else {
		return &_BondDiscountCurveMap.find(market)->second;
	}
}

SwaptionVolCube* MarketData::getSwaptionVolCube(enums::CurrencyEnum market){
	if ( _SwaptionVolCubeMap.find(market) == _SwaptionVolCubeMap.end()) {
		throw "Swaption vol cube not found in map!";
	}
	else {
		return &_SwaptionVolCubeMap.find(market)->second;
	}
}

FXSkewSurface* MarketData::getFXSkewSurface(enums::CurrencyEnum market){
	if ( _FXSkewSurfaceMap.find(market) == _FXSkewSurfaceMap.end()) {
		throw "FX skew not found in map!";
	}
	else {
		return &_FXSkewSurfaceMap.find(market)->second;
	}
}

