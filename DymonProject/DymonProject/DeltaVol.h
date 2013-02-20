//created by Wang Jianwei on 01 Dec 2012
#ifndef DELTAVOL_H
#define DELTAVOL_H
#include "Enums.h"

using namespace utilities;

namespace utilities {

	class DeltaVol {

	public:
		DeltaVol(){};
		DeltaVol(enums::VolType optionType, double delta, double tenorExpiry, double tenorDiscount, double vol){
			setVolType(optionType);
			setDelta(delta);
			setTenorExpiry(tenorExpiry);
			setTenorDiscount(tenorDiscount);
			setVol(vol);
		}
		~DeltaVol(){};

		// Getters and Setters
		double getDelta(){ return _delta; }
		double getVol(){ return _vol; }
		enums::VolType getVolType(){ return _optionType; }
		double getTenorExpiry(){ return _tenorExpiry; }
		double getTenorDiscount(){ return _tenorDiscount; }

		void setDelta(double delta){ _delta = delta; }
		void setVol(double vol) { _vol = vol; }
		void setVolType(enums::VolType optionType){ _optionType = optionType; }
		void setTenorExpiry(double tenorExpiry){ _tenorExpiry = tenorExpiry; }
		void setTenorDiscount(double tenorDiscount){ _tenorDiscount = tenorDiscount; }

	private:

		double _delta;
		double _vol;
		enums::VolType _optionType;
		double _tenorExpiry;
		double _tenorDiscount;
	};
}
#endif