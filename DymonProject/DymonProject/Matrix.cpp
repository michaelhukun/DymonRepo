//created by Hu Kun on 10 Jan 2013
#include "Matrix.h"

using namespace utilities;
using namespace std;

Matrix::Matrix(int rows, int cols) {
	setDimension(rows,cols);
}

Matrix::~Matrix() {
	delete [] _matrixEntry;
}

void Matrix::setDimension(int rows, int cols){
	_matrixEntry=new double[rows*cols];
	for(int i =0; i<rows*cols; i++){
		_matrixEntry[i]=0;
	}
}

Matrix Matrix::MatrixProduct(Matrix m2) {

	if (getNumOfCols()!=m2.getNumOfRows())
		throw "Matrix dimension not match!";

	Matrix mP(getNumOfRows(),m2.getNumOfCols());
	double tempResult;

	for (int i=0; i<=getNumOfRows()-1;i++) {
		for (int j=0;j<=m2.getNumOfCols()-1;j++) {
			tempResult=0;
			for (int k=0;k<=m2.getNumOfRows()-1;k++) {
				tempResult+=getMatrixElement(i*getNumOfCols(),k)*m2.getMatrixElement(k*m2.getNumOfCols(),j);
			}
			mP.setMatrixElement(tempResult,i,j);
		}
	}
	return mP;

}

Matrix Matrix::MatrixPlus(Matrix m2) {
	
	if (getNumOfCols()!=m2.getNumOfCols() || getNumOfRows()!=m2.getNumOfRows() )
		throw "Matrix dimension not match!";

	Matrix mPlus;

	return mPlus;
}

Matrix Matrix::MatrixMinus(Matrix m2) {
	
	if (getNumOfCols()!=m2.getNumOfCols() || getNumOfRows()!=m2.getNumOfRows() )
		throw "Matrix dimension not match!";

	Matrix mMinus;

	return mMinus;
}

double Matrix::getMatrixElement(int row, int col) {

	return _matrixEntry[row*getNumOfCols()+col];
}

void Matrix::setMatrixElement(double value, int row, int col) {
	_matrixEntry[row*getNumOfCols()+col]=value;
}

Matrix Matrix::inverse() {
	Matrix mInv;
	/*   for(i = 0; i < n; i++){
	for(j = n; j < 2*n; j++){
	if(i==(j-n))
	matrix[i][j] = 1.0;
	else
	matrix[i][j] = 0.0;
	}
	}
	for(i = 0; i < n; i++){
	for(j = 0; j < n; j++){
	if(i!=j){
	ratio = matrix[j][i]/matrix[i][i];
	for(k = 0; k < 2*n; k++){
	matrix[j][k] -= ratio * matrix[i][k];
	}
	}
	}
	}
	for(i = 0; i < n; i++){
	a = matrix[i][i];
	for(j = 0; j < 2*n; j++){
	matrix[i][j] /= a;
	}
	}*/

	return mInv;
}

Matrix Matrix::transpose() {
	Matrix mT(getNumOfCols(),getNumOfRows());

	for (int i=0; i<=mT.getNumOfRows()-1;i++) {
		for (int j=0;j<=mT.getNumOfCols()-1;j++) {
			mT.setMatrixElement(getMatrixElement(j,i),i,j);	
		}
	}
	return mT;
}

int Matrix::getNumOfRows() {
	return rows;
}

int Matrix::getNumOfCols() {
	return cols;
}

void Matrix::setNumOfRows(int rowsValue) {
	rows=rowsValue;
}

void Matrix::setNumOfCols(int colsValue) {
	cols=colsValue;
}

Matrix Matrix::operator+(Matrix &matrix){
	return MatrixPlus(matrix);
}

Matrix Matrix::operator-(Matrix &matrix){
	return MatrixMinus(matrix);
}

Matrix Matrix::operator*(Matrix &matrix){
	return MatrixProduct(matrix);
}

void Matrix::printMatrix() {
	for (int i=0; i<=rows-1;i++) {
		for (int j=0;j<=cols-1;j++) {
			cout<<_matrixEntry[i*cols+j]<<" ";
		}
		cout<<endl;
	}
}