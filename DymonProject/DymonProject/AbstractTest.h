//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

#include <string>
#include "date.h"
#include <tuple>
#include "AbstractFileSource.h"

using namespace std;
using namespace utilities;
using namespace DAO;

namespace UnitTest{
	class AbstractTest: public AbstractFileSource{

	public:

		typedef tuple<date, double> point;

		AbstractTest(){};
		~AbstractTest(){};

		virtual void init(Configuration* cfg){
			AbstractFileSource::init(cfg);
		};

		virtual void retrieveRecord()=0;

		virtual void runTest()=0;

	protected:

		bool compareResult(double derivedVal, double expectedVal);

		bool compareResult(std::string derivedVal, std::string expectedVal);

		std::string getString(point point0);

		double _EPSILON;
	};
}
#endif