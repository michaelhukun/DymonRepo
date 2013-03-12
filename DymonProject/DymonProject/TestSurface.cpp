#include "TestSurface.h"
#include "DiscountCurve.h"
#include "LinearInterpolator.h"
#include "date.h"
#include "AbstractSurface.h"

using namespace utilities;
using namespace UnitTest;

void TestSurface::runTest(){
	surfaceTestSuit();
}

void TestSurface::surfaceTestSuit(){
	surfaceTest();
}

void TestSurface::surfaceTest(){
	
	typedef tuple<date, double> point;
	date date0(2456295);
	date date1(2456296);
	date date2(2456297);
	point point1(date0, 1);
	point point2(date1, 2);
	point point3(date2, 2.5);
	DiscountCurve* yc1 = new DiscountCurve();
	LinearInterpolator<date>* li1 = new LinearInterpolator<date>(point1, point2);
	LinearInterpolator<date>* li2 = new LinearInterpolator<date>(point2, point3);
	yc1->insertLineSection(li1);
	yc1->insertLineSection(li2);
	point point4(date0, 10);
	point point5(date1, 20);
	point point6(date2, 25);
	DiscountCurve* yc2 = new DiscountCurve();
	LinearInterpolator<date>* li3 = new LinearInterpolator<date>(point4, point5);
	LinearInterpolator<date>* li4 = new LinearInterpolator<date>(point5, point6);
	yc2->insertLineSection(li3);
	yc2->insertLineSection(li4);

	AbstractSurface<double, date>* as = new AbstractSurface<double, date>();
	as->insertcurve(1,yc1);
	as->insertcurve(2,yc2);
	cout<<"Surface validation pass: "<<as->validateSurface()<<endl;
	cout<<as->toString()<<endl;

}