//created by Wang Jianwei on 01 Dec 2012
#ifndef FXFORWARD_H
#define FXFORWARD_H
#include "Enums.h"
#include "CcyPair.h"
#include "date.h"
#include "AbstractForward.h"

using namespace utilities;
using namespace instruments;

namespace utilities {

	class FXForward: public AbstractForward{

	public:
		FXForward():AbstractForward(){};
		~FXForward(){};

		// Getters and Setters
		CcyPair getCcyPair(){ return _ccyPair; }
		bool getIsSpot(){ return _isSpot; }

		void setCcyPair(CcyPair ccyPair){ _ccyPair = ccyPair; }
		void setIsSpot(bool isSpot){ _isSpot = isSpot; }

		std::string toString(){return "";};

	private:

		CcyPair _ccyPair;
		bool _isSpot;
	
	};
}
#endif