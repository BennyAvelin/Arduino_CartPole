#include"Matrix.h"
#include<stdlib.h>
#include<iostream>

double DNULL = 0.0;

Matrix::Matrix(){
	matrix = NULL;
	rows = columns = totalSize = 0;
}

Matrix::Matrix( double *input, int rows, int columns) {
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (double *) malloc(sizeof(double)*rows*columns);
	memcpy(this->matrix,input,sizeof(double)*totalSize);
}

Matrix::Matrix( const Matrix &obj) {
	this->rows = obj.rows;
	this->columns = obj.columns;
	this->totalSize = rows*columns;
	this->matrix = (double *) malloc(sizeof(double)*rows*columns);
	memcpy(this->matrix,obj.matrix,sizeof(double)*totalSize);
}

Matrix::Matrix( int rows, int columns) {
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (double *) malloc(sizeof(double)*rows*columns);
}

Matrix::Matrix( int rows, int columns, double initialValue){
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (double *) malloc(sizeof(double)*rows*columns);
	for (int index = 0; index < totalSize; index++) {
		matrix[index] = initialValue;
	}
}

Matrix::~Matrix(){
	free(this->matrix);
	std::cout << "Delete" << std::endl;
}

double &Matrix::get(int row, int column){
	if (0 <= row && row < rows && 0 <= column && column < columns) 
		return matrix[row*columns+column];
	return DNULL;
} //Gets a pointer to the location holding the value at position row, 
//column for setting.


double &Matrix::operator[](int index){
	if (0 <= index && index < totalSize)
		return matrix[index];
	return matrix[0]; // Return something i.e. the first element
} //Gets a pointer to the location holding the

Matrix Matrix::operator+(Matrix &B){
	Matrix retval (rows,columns);
	if (B.totalSize != totalSize) return Matrix(0,0);
	for (int index = 0; index < totalSize; index++){
		retval[index] = this->matrix[index] + B.matrix[index];
	}
	return retval;
}

Matrix Matrix::operator-(Matrix &B){
	Matrix retval (rows,columns);
	if (B.totalSize != totalSize) return Matrix(0,0);
	for (int index = 0; index < totalSize; index++){
		retval[index] = this->matrix[index] - B.matrix[index];
	}
	return retval;
}


Matrix Matrix::operator*(Matrix &B){
	//Check input output size
	if (this->columns != B.rows) return Matrix(0,0);
	Matrix retval (this->rows,B.columns);
	for (int row = 0; row < retval.rows; row++){
		for (int col=0; col < retval.columns; col++){
			//Make the dot-product between row[row] from this
			//and column[col] from B
			int n = this->columns; //This is how long the vectors are
			double result = 0;
			//Compute the scalar product and put the result in result.
			for (int index = 0; index < n; index++){
				result+=get(row,index)*B.get(index,col);
			}
			retval.get(row,col) = result;
		}
	}
	return retval;
}

Matrix Matrix::operator*(double scale) {
	Matrix retval (rows,columns);
	for (int index = 0; index < totalSize; index++){
		retval[index] = scale*(this->matrix[index]);
	}
	return retval;
}

Matrix Matrix::pm(Matrix &B) {
	Matrix retval (rows,columns);
	if (B.totalSize != totalSize) return Matrix(0,0);
	for (int index = 0; index < totalSize; index++){
		retval[index] = this->matrix[index] * B.matrix[index];
	}
	return retval;
}

Matrix Matrix::apply(double (*f)(double)) {
	Matrix retval (rows,columns);
	for (int index = 0; index < totalSize; index++){
		retval[index] = f(this->matrix[index]);
	}
	return retval;
}

Matrix Matrix::transpose() {
	Matrix retval (columns,rows);
	for (int row = 0; row < rows; row++){
		for (int col = 0; col < columns; col++){
			retval.get(col,row) = get(row,col);
		}
	}
	return retval;
}

Tuple Matrix::shape(){
	return Tuple(rows,columns);
}

void Matrix::print(){
	std::cout << "Matrix :" << this->rows << " " << this->columns << std::endl;
	for (int x = 0; x<rows; x++) {
		for (int y = 0; y < columns; y++ ) {
			std::cout << get(x,y) << "\t";
		}
		std::cout << std::endl;
	}
}