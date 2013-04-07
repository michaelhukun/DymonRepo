//created by Wang Jianwei on 01 Dec 2012
#ifndef DiscountCurve_H
#define DiscountCurve_H
#include "AbstractCurve.h"
#include "AbstractInstrument.h"
#include <vector>
#include "Enums.h"
#include "date.h"

using namespace instruments;

namespace utilities{
	class DiscountCurve: public AbstractCurve<date>{
		
	public:
		
		DiscountCurve():AbstractCurve(){
			_curveRateType = enums::DF;
		};

		DiscountCurve(std::vector<AbstractInterpolator<date>*>* lineSectionVector):AbstractCurve(lineSectionVector){};

		double getDiscountFactor(date endDate);

		double getDiscountFactor(date beginDate, date endDate);

		double getDiscountFactor(date endDate, double zeroRate);

		double getZeroRate(date endDate);

		double getZeroRate(date endDate, double discountFactor);

		double getZeroRate(date aDate, enums::DayCountEnum dayCount);

		double getZeroRate(date beginDate, date endDate, enums::DayCountEnum dayCount);

		double getZeroRate(date beginDate, date endDate, double discountFactor, enums::DayCountEnum dayCount);

		double getDFChangingZeroRate(date aDate, double zeroRateDiff, enums::DayCountEnum dayCount);

		double getFLiborRate(date forwardStartDate,date forwardEndDate,enums::DayCountEnum dayCount);

		void insertComponent(AbstractInstrument* instrument){ _components.push_back(instrument); }

		void deleteLastComponent(){	_components.erase(_components.end()-1); }

		bool existComponent(AbstractInstrument* instrument){ 
			for(unsigned int i = 0; i<_components.size(); i++){
				if (_components.at(i)->getID() == instrument->getID())
					return true;
			}
			return false;
		}

		void dumpComponentNames();

		//getters and setters

		vector<AbstractInstrument*>* getComponents() {return &_components; }

		void setComponents(vector<AbstractInstrument*> components) { _components = components; }

	private:

		vector<AbstractInstrument*> _components;
	};
}
#endif