//created by Wang Jianwei on 01 Dec 2012
#ifndef ABSTRACTCURVE_H
#define ABSTRACTCURVE_H
#include "AbstractInterpolator.h"
#include "AbstractDataStructure.h"
#include <vector>
#include <sstream>

using namespace std;

namespace utilities{
	template<typename T> class AbstractCurve: public AbstractDataStructure{

	public:

		typedef tuple<T, double> point;

		AbstractCurve():AbstractDataStructure(){
			_lineSectionVector = new std::vector<AbstractInterpolator<T>*>;
		};

		AbstractCurve(std::vector<AbstractInterpolator<T>*>* lineSectionVector){
			setLineSectionVector(lineSectionVector);
		}

		virtual void insertLineSection(AbstractInterpolator<T>* lineSection);

		virtual double getValue(T xVal);

		point getCurveStartPoint();

		point getCurveEndPoint();

		void setLineSectionVector(std::vector<AbstractInterpolator<T>*>* lineSectionVector){
			_lineSectionVector = lineSectionVector;		
		}

		bool validateLineSections();

		virtual std::string toString();

	private:

		std::vector<AbstractInterpolator<T>*>* _lineSectionVector;

	};

	template<typename T>
	tuple<T, double> AbstractCurve<T>::getCurveStartPoint(){
		if (_lineSectionVector->size()>0){
			AbstractInterpolator<T>* startLineSection = _lineSectionVector->at(0);
			return startLineSection->getStartPoint();
		}
		throw "Curve is still empty.";
	}

	template<typename T>
	tuple<T, double> AbstractCurve<T>::getCurveEndPoint(){
		int curveSize = _lineSectionVector->size();
		if (curveSize>0){
			AbstractInterpolator<T>* endLineSection = _lineSectionVector->at(curveSize-1);
			return endLineSection->getEndPoint();
		}
		throw "Curve is still empty.";
	}

	template<typename T>
	void AbstractCurve<T>::insertLineSection(AbstractInterpolator<T>* lineSection){
		if (_lineSectionVector->size()==0){
			_lineSectionVector->insert(_lineSectionVector->begin(),lineSection);
		} else{
			int vectorSize = _lineSectionVector->size();
			_lineSectionVector->insert(_lineSectionVector->begin()+vectorSize,lineSection);
		}
	}

	template<typename T>
	double AbstractCurve<T>::getValue(T xVal){
		// xVal is smaller than the starting point of the curve.
		if (xVal<std::get<0>(getCurveStartPoint())){
			return std::get<1>(getCurveStartPoint());
		}
		
		// xVal is larger than the ending point of the curve.
		if (xVal>std::get<0>(getCurveEndPoint())){
			return std::get<1>(getCurveEndPoint());
		}

		T startX, endX;
		for (unsigned int i = 0; i<_lineSectionVector->size(); i++){
			AbstractInterpolator<T>* currentLine = _lineSectionVector->at(i);
			startX = currentLine->getStartingX();
			endX = currentLine->getEndingX();
			if (startX<=xVal && xVal<=endX){
				AbstractInterpolator<T>* ai= _lineSectionVector->at(i);
				point pointOnCurve = ai->interpolate(xVal);
				return std::get<1>(pointOnCurve);
			}
		}

		throw "Point not found on curve for X value: ";
	}

	template<typename T>
	bool AbstractCurve<T>::validateLineSections(){
		bool validationPass = true;
		for (unsigned int i = 1; i<=_lineSectionVector->size(); i++){
			AbstractInterpolator<T>* nextLine = _lineSectionVector->at(i);
			AbstractInterpolator<T>* previousLine = _lineSectionVector->at(i);
			T nextStartX = nextLine->getStartingX();
			T previousEndX = previousLine->getEndingX();
			if (nextStartX!=previousEndX){
				cout<<"Curve gap detected: "<<previousEndX<< " " <<nextStartX<<endl;
				validationPass = false;
			}
		}
		return validationPass;
	}

	template<typename T>
	string AbstractCurve<T>::toString(){
		std::stringstream ss (stringstream::in | stringstream::out);
		ss << "Curve - Line sections: \n";
		for (unsigned int i = 0; i<_lineSectionVector->size(); i++){
			ss << _lineSectionVector->at(i)->toString() <<"\n";
		}
		return ss.str();
	}

	template<>
	inline bool AbstractCurve<date>::validateLineSections(){
		bool validationPass = true;
		for (unsigned int i = 1; i<_lineSectionVector->size(); i++){
			AbstractInterpolator<date>* nextLine = _lineSectionVector->at(i);
			AbstractInterpolator<date>* previousLine = _lineSectionVector->at(i-1);
			date nextStartX = nextLine->getStartingX();
			date previousEndX = previousLine->getEndingX();
			if (nextStartX.getJudianDayNumber()!=previousEndX.getJudianDayNumber()){
				cout<<"Curve gap detected: "<<previousEndX.toString()<< " " <<nextStartX.toString()<<endl;
				validationPass = false;
			}
		}
		return validationPass;
	}


}
#endif