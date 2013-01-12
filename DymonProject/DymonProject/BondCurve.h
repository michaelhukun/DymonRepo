//created by Wang Jianwei on 01 Dec 2012
#ifndef BONDCURVE_H
#define BONDCURVE_H
#include "AbstractCurve.h"
#include <vector>
#include "Enums.h"
#include "date.h"

namespace utilities{
	class BondCurve: public AbstractCurve<date>{
		
	public:
		
		BondCurve():AbstractCurve(){};

		BondCurve(std::vector<AbstractInterpolator<date>*>* lineSectionVector):AbstractCurve(lineSectionVector){};
		
		double getDiscountFactor(date Date0){
			getValue(Date0);
		};

		std::string toString(){
			return "\nBond "+AbstractCurve::toString();
		};
	};
}
#endif