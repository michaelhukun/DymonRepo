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

	};
}
#endif