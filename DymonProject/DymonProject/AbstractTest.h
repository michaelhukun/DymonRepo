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

		// Constructors
		AbstractTest(){};
		~AbstractTest(){};

		// Methods
		virtual void init(Configuration* cfg){	AbstractFileSource::init(cfg);	};

		virtual void retrieveRecord(){};

		virtual void runTest()=0;

		// Getters and Setters
		bool isEnabled(){ return _isEnabled; }
		std::string getTestName(){ return _testName; }

		void setIsEnabled(bool isEnabled){ _isEnabled = isEnabled; }
		void setTestName(std::string testName){ _testName = testName; }

	protected:

		bool compareResult(double derivedVal, double expectedVal);

		bool compareResult(std::string derivedVal, std::string expectedVal);

		std::string getString(point point0);

		double _EPSILON;
		bool _isEnabled;
		std::string _testName;
	};
}
#endif