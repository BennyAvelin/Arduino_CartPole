#include"Matrix.h"
#include<stdlib.h>
#ifndef X86
#include<Arduino.h>
#else
#include<iostream>
#endif

float DNULL = 0.0;

Matrix::Matrix(){
	matrix = NULL;
	rows = columns = totalSize = 0;
}

Matrix::Matrix( float *input, int rows, int columns) {
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
	memcpy(this->matrix,input,sizeof(float)*totalSize);
}

Matrix::Matrix( const Matrix &obj) {
	this->rows = obj.rows;
	this->columns = obj.columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
	memcpy(this->matrix,obj.matrix,sizeof(float)*totalSize);
}

Matrix::Matrix( int rows, int columns) {
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
}

Matrix::Matrix( int rows, int columns, float initialValue){
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
	for (int index = 0; index < totalSize; index++) {
		matrix[index] = initialValue;
	}
}

Matrix::~Matrix(){
	free(this->matrix);
}

float &Matrix::get(int row, int column){
	if (0 <= row && row < rows && 0 <= column && column < columns) 
		return matrix[row*columns+column];
	return DNULL;
} //Gets a pointer to the location holding the value at position row, 
//column for setting.


float &Matrix::operator[](int index){
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
			float result = 0;
			//Compute the scalar product and put the result in result.
			for (int index = 0; index < n; index++){
				result+=get(row,index)*B.get(index,col);
			}
			retval.get(row,col) = result;
		}
	}
	return retval;
}

Matrix Matrix::operator*(float scale) {
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

Matrix Matrix::apply(float (*f)(float)) {
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
#ifndef X86
	Serial.print("Matrix :");
	Serial.print(this->rows);
	Serial.print(" ");
	Serial.print(this->columns);
	Serial.print("\n");
#else
	std::cout << "Matrix :" << this->rows << " " << this->columns;
	std::cout << std::endl;
#endif
	for (int x = 0; x<rows; x++) {
		for (int y = 0; y < columns; y++ ) {
#ifndef X86
			Serial.print(get(x,y));
			Serial.print("\t");
#else
			std::cout << get(x,y) << "\t";
#endif
		}
#ifndef X86
		Serial.print("\n");
#else
		std::cout << std::endl;
#endif
	}
}
