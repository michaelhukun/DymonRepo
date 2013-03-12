//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTBUILDER_H
#define ABSTRACTBUILDER_H
#include "AbstractSession.h"
#include "AbstractDataStructure.h"

using namespace Session;

namespace utilities{
	class AbstractBuilder: public AbstractSession{
		
	public:
		
		AbstractBuilder(){}
		
		virtual void init(Configuration* cfg){}
		
		virtual AbstractDataStructure* build(Configuration* cfg)=0;
		
		// Getters and Setters
		enums::interpolAlgo getInterpolAlgo(){return _interpolAlgo;}
		void setInterpolAlgo(enums::interpolAlgo interpolAlgo){_interpolAlgo=interpolAlgo;}

		enums::NumericAlgo getNumericalAlgo(){return _numericalAlgo;}
		void setNumericalAlgo(enums::NumericAlgo numericalAlgo){_numericalAlgo=numericalAlgo;}

	protected:
		enums::interpolAlgo _interpolAlgo;
		enums::NumericAlgo _numericalAlgo;
	};
}
#endif