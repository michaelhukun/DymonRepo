//created by Wang Jianwei on 01 Dec 2012
#ifndef TESTMANAGER_H
#define TESTMANAGER_H
#include <string>
#include "AbstractSession.h"

using namespace utilities;

namespace Session {
	class TestManager: public AbstractSession{

	public:
		void init(Configuration*);

		void start();

		void stop();

	private:	
	};
}

#endif