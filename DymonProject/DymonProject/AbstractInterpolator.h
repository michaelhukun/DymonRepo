//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTINTERPOLATOR_H
#define ABSTRACTINTERPOLATOR_H
#include <vector>
#include <tuple>
#include "date.h"
#include "Enums.h"
#include <sstream>
#include "RateConverter.h"

using namespace std;
using namespace utilities;

namespace utilities{
	template<typename T> class AbstractInterpolator{

	public:

		typedef tuple<T, double> Point;

		AbstractInterpolator(){}

		AbstractInterpolator(Point startPoint, Point endPoint){
			_startPoint = startPoint;
			_endPoint = endPoint;
			addCurveConfig(enums::RateTypeNull, enums::RateTypeNull, enums::DayCountNull, date());
		}

      // Methods
		void addCurveConfig(enums::RateType curveRateType, enums::RateType interpolRateType, enums::DayCountEnum dayCount, date startDate){
			_curveRateType = curveRateType;
			_interpolRateType = interpolRateType;
			_dayCount = dayCount;
			_startDate = startDate;
		}
		virtual Point interpolate(T xVal){
			xValInRangeCheck(xVal);
			if (xVal == std::get<0>(_startPoint)) return _startPoint;
			if (xVal == std::get<0>(_endPoint)) return _endPoint;
			Point convertedStartPoint = convertToInterpolRate(_startPoint);
			Point convertedEndPoint = convertToInterpolRate(_endPoint);
			Point convertedMidPoint = interpolateConverted(convertedStartPoint, convertedEndPoint, xVal);
			Point curveMidPoint = convertToCurveRate(convertedMidPoint);
			return curveMidPoint;
		}
		virtual Point interpolateConverted(Point convertedStartPoint, Point convertedEndPoint, T xVal)=0;
		virtual std::string toString();
		Point convertToInterpolRate(Point point){ return point; }
		Point convertToCurveRate(Point point){ return point; }

		// Getters and Setters
		Point getStartPoint(){return _startPoint;}
		Point getEndPoint(){return _endPoint;}
		T getStartingX(){ return std::get<0>(_startPoint);}
		T getEndingX(){	return std::get<0>(_endPoint);	}
		enums::RateType getPointRateType(){ return _pointRateType; }
		enums::RateType getInterpolRateType(){ return _interpolRateType; }

		void setStartPoint(Point startPoint){_startPoint=startPoint;}
		void setEndPoint(Point endPoint){_endPoint=endPoint;}
		void setPointRateType(enums::RateType pointRateType){ _pointRateType = pointRateType; }
		void setInterpolRateType(enums::RateType interpolRateType){ _interpolRateType = interpolRateType; }

	protected:

		void xValInRangeCheck(T xVal);
		Point _startPoint;
		Point _endPoint;
		enums::interpolAlgo _algo;
		enums::RateType _curveRateType;
		enums::RateType _interpolRateType;
		enums::DayCountEnum _dayCount;
		date _startDate;
	};
	
	template <class T> 
	string AbstractInterpolator<T>::toString(){
		std::stringstream ss (stringstream::in | stringstream::out);
		ss << "line section from {";
		ss << std::get<0>(_startPoint) << "," <<std::get<1>(_startPoint)<<"} to {";
		ss << std::get<0>(_endPoint) << "," <<std::get<1>(_endPoint)<<"}";
		return ss.str();
	}

	template <class T> 
	void AbstractInterpolator<T>::xValInRangeCheck(T xVal){
		if (xVal<AbstractInterpolator<T>::getStartingX()){
			std::stringstream ss (stringstream::in | stringstream::out);
			ss<<"X Value is smaller than the line section starting value";
			throw ss.str();
		}else if (xVal>AbstractInterpolator<T>::getEndingX()){
			std::stringstream ss (stringstream::in | stringstream::out);
			ss<<"X Value is larger than the line section ending value";
			throw ss.str();
		}
	}
	
	template <> 
	inline string AbstractInterpolator<date>::toString(){
		std::stringstream ss (stringstream::in | stringstream::out);
		ss << "line section from {";
		ss << std::get<0>(_startPoint).toString() << "," <<std::get<1>(_startPoint)<<"} to {";
		ss << std::get<0>(_endPoint).toString() << "," <<std::get<1>(_endPoint)<<"}";
		return ss.str();
	}

	template <> 
	inline AbstractInterpolator<date>::Point AbstractInterpolator<date>::convertToInterpolRate(Point point){
		date endDate = std::get<0>(point);
		double curveRate = std::get<1>(point);
		RateConverter rc(_startDate, _dayCount);
		double interpolRate = rc.convertRate(endDate, curveRate, _curveRateType, _interpolRateType);
		return Point(endDate, interpolRate);
	}

	template <> 
	inline AbstractInterpolator<date>::Point AbstractInterpolator<date>::convertToCurveRate(Point point){
		date endDate = std::get<0>(point);
		double interpolRate = std::get<1>(point);
		RateConverter rc(_startDate, _dayCount);
		double curveRate = rc.convertRate(endDate, interpolRate, _interpolRateType, _curveRateType);
		return Point(endDate, curveRate);
	}
}
#endif