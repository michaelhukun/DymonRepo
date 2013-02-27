//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTSESSION_H
#define ABSTRACTSESSION_H
#include <string>
#include <map>
#include "Configuration.h"

using namespace utilities;

namespace Session{
	class AbstractSession{

	public:
		virtual void init(Configuration*)=0;

		virtual void start(){};

		virtual void stop(){};
	};
}
#endif