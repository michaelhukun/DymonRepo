//created by Hu Kun 30 Nov 2012
#include "MarketData.h"
#include "Configuration.h"
#include "SwapCurveBuilder.h"
#include "SwaptionVolCubeBuilder.h"
#include "FXSkewSurfaceBuilder.h"
#include "FXForwardImpliedCurveBuilder.h"
#include "BondCurveBuilder.h"
#include "RecordHelper.h"
#include "Configuration.h"
#include "ForwardCurveBuilder.h"
#include "fileUtil.h"

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
	buildFXForwardImpliedCurve();
	buildFXSkewSurface();
}

void MarketData::buildSwapDiscountCurve(){
	vector<string> marketVector = FileUtil::split(Configuration::getInstance()->getProperty("swapCurve.build.enabled",true,""), ',');
	if (marketVector.size()==0) return;

	cout << "\n******** Build Swap Discount Curve ********\n" << endl;

	for (unsigned int i = 0; i<marketVector.size(); i++ ){
		CurrencyEnum ccyEnum = EnumHelper::getCcyEnum(marketVector.at(i));
		SwapCurveBuilder* builder = new SwapCurveBuilder(ccyEnum);
		DiscountCurve* curve = builder->build(Configuration::getInstance());
		_SwapDiscountCurveMap.insert(pair<CurrencyEnum, DiscountCurve>(ccyEnum, *curve));
		cout<<curve->toExcelFriendlyString()<<endl;
	}
}

void MarketData::buildBondDiscountCurve(){
   vector<string> marketVector = FileUtil::split(Configuration::getInstance()->getProperty("bondCurve.build.enabled",true,""), ',');
	if (marketVector.size()==0) return;

	cout << "\n******** Build Bond Discount Curve ********\n" << endl;
	for (unsigned int i = 0; i<marketVector.size(); i++ ){
		CurrencyEnum ccyEnum = EnumHelper::getCcyEnum(marketVector.at(i));
		BondCurveBuilder* builder = new BondCurveBuilder(ccyEnum);
		DiscountCurve* curve = builder->build(Configuration::getInstance());
		_BondDiscountCurveMap.insert(pair<CurrencyEnum, DiscountCurve>(ccyEnum, *curve));
		cout<<curve->toExcelFriendlyString()<<endl;
	}
}

void MarketData::buildFXForwardImpliedCurve(){
	bool enabled = Configuration::getInstance()->getProperty("FXForwardImplied.build.enabled",true,"")=="true"?true:false;
	if (!enabled) return;

	cout << "\n******** Build FX Forward Implied Discount Curve ********\n" << endl;
	RecordHelper::FXForwardMap::iterator it;
	RecordHelper::FXForwardMap* rateMap = RecordHelper::getInstance()->getFXForwardMap();
	for (it = rateMap->begin(); it!= rateMap->end(); ++it){
		string ccyPairStr = it->first;
		FXForwardImpliedCurveBuilder* builder = new FXForwardImpliedCurveBuilder(ccyPairStr);
		DiscountCurve* curve = builder->build(Configuration::getInstance());
		enums::CurrencyEnum forwardImpliedCurveCcy = builder->getForwardImpliedCurveCcy();
		_FXForwardImpliedCurveMap.insert(pair<CurrencyEnum, DiscountCurve>(forwardImpliedCurveCcy, *curve));
		cout<<curve->toExcelFriendlyString()<<endl;
	}
}

void MarketData::buildSwaptionVolCube(){
	bool enabled = Configuration::getInstance()->getProperty("swaptionVolCube.build.enabled",true,"")=="true"?true:false;
	if (!enabled) return;

	cout << "\n******** Build Swaption Vol Cube ********\n" << endl;
	SwaptionVolCubeBuilder* builder = new SwaptionVolCubeBuilder(USD);
	SwaptionVolCube* cube = builder->build(Configuration::getInstance());
	_SwaptionVolCubeMap.insert(pair<CurrencyEnum, SwaptionVolCube>(USD, *cube));
}

void MarketData::buildFXSkewSurface(){
	bool enabled = Configuration::getInstance()->getProperty("FXSkew.build.enabled",true,"")=="true"?true:false;
	if (!enabled) return;

	cout << "\n******** Build FX Skew Surface ********\n" << endl;
	RecordHelper::FXVolSkewMap::iterator it;
	RecordHelper::FXVolSkewMap* rateMap = RecordHelper::getInstance()->getFXVolSkewMap();
	for (it = rateMap->begin(); it!= rateMap->end(); ++it){
		string ccyPairStr = it->first;
		FXSkewSurfaceBuilder* builder = new FXSkewSurfaceBuilder(ccyPairStr);
		FXSkewSurface* surface = builder->build(Configuration::getInstance());
		_FXSkewSurfaceMap.insert(pair<string, FXSkewSurface>(ccyPairStr, *surface));
		cout<<surface->dumpSruface(10)<<endl;
	}
}

void MarketData::buildForwardCurve(){
	bool enabled = Configuration::getInstance()->getProperty("forwardCurve.build.enabled",true,"")=="true"?true:false;
	if (!enabled) return;

	cout << "\n******** Build FX Forward curve ********\n" << endl;
	RecordHelper::FXForwardMap::iterator it;
	RecordHelper::FXForwardMap* rateMap = RecordHelper::getInstance()->getFXForwardMap();
	for (it = rateMap->begin(); it!= rateMap->end(); ++it){
		string ccyPairStr = it->first;
		ForwardCurveBuilder* builder = new ForwardCurveBuilder(ccyPairStr);
		AbstractCurve<date>* curve = builder->build(Configuration::getInstance());
		_ForwardCurveMap.insert(pair<string, AbstractCurve<date>>(ccyPairStr, *curve));
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
	} else {
		return &_BondDiscountCurveMap.find(market)->second;
	}
}

DiscountCurve* MarketData::getFXForwardImpliedCurve(enums::CurrencyEnum market){
	if ( _FXForwardImpliedCurveMap.find(market) == _FXForwardImpliedCurveMap.end()) {
		throw "FX Forward Implied curve not found in map!";
	} else {
		return &_FXForwardImpliedCurveMap.find(market)->second;
	}
}

SwaptionVolCube* MarketData::getSwaptionVolCube(enums::CurrencyEnum market){
	if ( _SwaptionVolCubeMap.find(market) == _SwaptionVolCubeMap.end()) {
		throw "Swaption vol cube not found in map!";
	} else {
		return &_SwaptionVolCubeMap.find(market)->second;
	}
}

FXSkewSurface* MarketData::getFXSkewSurface(std::string ccyPairStr){
	if ( _FXSkewSurfaceMap.find(ccyPairStr) == _FXSkewSurfaceMap.end()) {
		throw "FX skew not found in map!";
	} else {
		return &_FXSkewSurfaceMap.find(ccyPairStr)->second;
	}
}

AbstractCurve<date>* MarketData::getForwardCurve(std::string ccyPairStr){
	if ( _ForwardCurveMap.find(ccyPairStr) == _ForwardCurveMap.end()) {
		throw "Forward curve not found in map!";
	} else {
		return &_ForwardCurveMap.find(ccyPairStr)->second;
	}
}