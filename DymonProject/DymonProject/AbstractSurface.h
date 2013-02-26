//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTSURFACE_H
#define ABSTRACTSURFACE_H
#include "AbstractCurve.h"
#include "AbstractInterpolator.h"
#include "AbstractDataStructure.h"
#include "Configuration.h"
#include <map>
#include <sstream>

namespace utilities{
	template<typename U, typename T> class AbstractSurface: public AbstractDataStructure{
		
	public:		

		AbstractSurface():AbstractDataStructure(){
			_cfg = Configuration::getInstance(); 
			_curves = new std::map<U, AbstractCurve<T>*>;
		};

		virtual void insertcurve(U majorAxisVal, AbstractCurve<T>* curve);

		virtual AbstractCurve<T>* getcurve(U majorAxisVal);

		virtual double getValue(U majorAxisVal, T minorAxisVal);

		bool validateSurface();

		virtual std::string toString();

		virtual std::string toString(double majorAxisInterval);

	protected:

		std::map<U, AbstractCurve<T>*>* _curves;
		enums::interpolAlgo _majorAxisInterpol;
		Configuration* _cfg;

	};

	template<typename U, typename T>
	void AbstractSurface<U, T>::insertcurve(U majorAxisVal, AbstractCurve<T>* curve){
		_curves->insert(pair<U, AbstractCurve<T>*>(majorAxisVal,curve));
	}
	
	template<typename U, typename T>
	AbstractCurve<T>* AbstractSurface<U, T>::getcurve(U majorAxisVal){
		if (_curves->find(majorAxisVal) == _curves->end())
			throw "Major axis value not found on surface";
		return _curves->find(majorAxisVal)->second;
	}

	template<typename U, typename T>
	double AbstractSurface<U, T>::getValue(U majorAxisVal, T minorAxisVal){
		AbstractCurve<T>* curve=getcurve(majorAxisVal);
		return curve->getValue(minorAxisVal);
	}

	template<typename U, typename T>
	bool AbstractSurface<U, T>::validateSurface(){
		bool validationPass = true;
		for (map<U, AbstractCurve<T>*>::iterator it = _curves->begin(); it!=_curves->end(); it++)
			if (!(*it).second->validateLineSections())
				validationPass = false;
		return validationPass;
	}

	template<typename U, typename T>
	std::string  AbstractSurface<U, T>::toString(){
		std::stringstream ss (stringstream::in | stringstream::out);
		for (map<U, AbstractCurve<T>*>::iterator it = _curves->begin(); it!=_curves->end(); it++){
			ss<<"Major axis value ["<<(*it).first<<"] has ";
			ss<<(*it).second->toString();
		}
		return ss.str();
	}

	template<typename U, typename T>
	std::string  AbstractSurface<U, T>::toString(double majorAxisInterval){
		std::stringstream ss (stringstream::in | stringstream::out);
		for (map<U, AbstractCurve<T>*>::iterator it = _curves->begin(); it!=_curves->end(); it++){
			ss<<"Major axis value ["<<(*it).first<<"] has ";
			ss<<(*it).second->toString(majorAxisInterval);
		}
		return ss.str();
	}
}
#endif