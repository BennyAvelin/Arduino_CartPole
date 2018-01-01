#include"Matrix.h"
#include<stdlib.h>

#ifndef X86
/**
 * NULLCHECK, checks if there was a problem assigning memory to create this matrix
 * and in turn, puts the built in led on as an outside indicator that a memory problem
 * has occured.
 */
#define NULLCHECK if (this->matrix == NULL && rows*columns != 0) digitalWrite(LED_BUILTIN, HIGH);
#include<Arduino.h>
#include "BinarySerial.h"

#else

#define NULLCHECK
#include<iostream>

#endif

float DNULL = 0.0;

Matrix::Matrix(){
#ifdef X86
  std::cout << "Matrix constructed empty" << std::endl;
#endif
	matrix = NULL;
	rows = columns = totalSize = 0;
}

Matrix::Matrix( float *input, short rows, short columns) {
#ifdef X86
  std::cout << "Matrix constructed input,rows,columns" << std::endl;
#endif
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
  NULLCHECK
	memcpy(this->matrix,input,sizeof(float)*totalSize);
}

Matrix::Matrix( const Matrix &obj) {
#ifdef X86
  std::cout << "Matrix copied" << std::endl;
#endif
	this->rows = obj.rows;
	this->columns = obj.columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
  NULLCHECK
	memcpy(this->matrix,obj.matrix,sizeof(float)*totalSize);
}

Matrix::Matrix( short rows, short columns) {
#ifdef X86
  std::cout << "Matrix constructed with empty entries" << std::endl;
#endif
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
  NULLCHECK
}

Matrix::Matrix( short rows, short columns, float initialValue){
#ifdef X86
  std::cout << "Matrix constructed with specified values" << std::endl;
#endif
	this->rows = rows;
	this->columns = columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
  NULLCHECK
	for (short index = 0; index < totalSize; index++) {
		matrix[index] = initialValue;
	}
}

Matrix::~Matrix(){
#ifdef X86
	std::cout << "Matrix being destructed" << this << std::endl;
#endif
	if (matrix != NULL) {
		free(this->matrix);
  	  	matrix = NULL;
	}
}

float &Matrix::set(short row, short column){
	if (0 <= row && row < rows && 0 <= column && column < columns) 
		return matrix[row*columns+column];
	return DNULL;
} //Gets a poshorter to the location holding the value at position row, 
//column for setting.

float Matrix::get(short row, short column) const{
  if (0 <= row && row < rows && 0 <= column && column < columns) 
    return matrix[row*columns+column];
  return DNULL;
} //Gets a poshorter to the location holding the value at position row, 
//column for setting.


float &Matrix::operator[](short index){
	if (0 <= index && index < totalSize)
		return matrix[index];
	return matrix[0]; // Return something i.e. the first element
} //Gets a poshorter to the location holding the

Matrix Matrix::operator+(const Matrix &B) const{
	Matrix retval (rows,columns);
	if (B.totalSize != totalSize) return Matrix(0,0);
	for (short index = 0; index < totalSize; index++){
		retval[index] = this->matrix[index] + B.matrix[index];
	}
	return retval;
}

Matrix Matrix::operator-(const Matrix &B) const{
	Matrix retval (rows,columns);
	if (B.totalSize != totalSize) return Matrix(0,0);
	for (short index = 0; index < totalSize; index++){
		retval[index] = this->matrix[index] - B.matrix[index];
	}
	return retval;
}


Matrix Matrix::operator*(const Matrix &B) const{
	//Check input output size
	if (this->columns != B.rows) return Matrix(0,0);
	Matrix retval (this->rows,B.columns);
	for (short row = 0; row < retval.rows; row++){
		for (short col=0; col < retval.columns; col++){
			//Make the dot-product between row[row] from this
			//and column[col] from B
			short n = this->columns; //This is how long the vectors are
			float result = 0;
			//Compute the scalar product and put the result in result.
			for (short index = 0; index < n; index++){
				result+=get(row,index)*B.get(index,col);
			}
			retval.set(row,col) = result;
		}
	}
	return retval;
}

Matrix Matrix::operator*(const float scale) const{
	Matrix retval (rows,columns);
	for (short index = 0; index < totalSize; index++){
		retval[index] = scale*(this->matrix[index]);
	}
	return retval;
}

Matrix& Matrix::operator=(const Matrix &obj){
#ifdef X86
	std::cout << "Matrix constructed by equals" << this << &obj << std::endl;
#endif
	if (matrix != NULL) {
  	  free(matrix);
	  matrix = NULL;
  	}
  
	this->rows = obj.rows;
	this->columns = obj.columns;
	this->totalSize = rows*columns;
	this->matrix = (float *) malloc(sizeof(float)*rows*columns);
  NULLCHECK
	memcpy(this->matrix,obj.matrix,sizeof(float)*totalSize);
	return *this;
}

Matrix Matrix::pm(const Matrix &B) const{
	Matrix retval (rows,columns);
	if (B.totalSize != totalSize) return Matrix(0,0);
	for (short index = 0; index < totalSize; index++){
		retval[index] = this->matrix[index] * B.matrix[index];
	}
	return retval;
}

Matrix Matrix::apply(float (*f)(float)) const{
	Matrix retval (rows,columns);
	for (short index = 0; index < totalSize; index++){
		retval[index] = f(this->matrix[index]);
	}
	return retval;
}

Matrix Matrix::transpose() const{
	Matrix retval (columns,rows);
	for (short row = 0; row < rows; row++){
		for (short col = 0; col < columns; col++){
			retval.set(col,row) = get(row,col);
		}
	}
	return retval;
}

Tuple Matrix::shape() const{
	return Tuple(rows,columns);
}

float *Matrix::getRawData(){
  return matrix;
}

void Matrix::print() const{
#ifndef X86
	Serial.print(F("Matrix :"));
	Serial.print(this->rows);
	Serial.print(F(" "));
	Serial.print(this->columns);
	Serial.print(F("\n"));
	for (short x = 0; x<rows; x++) {
		for (short y = 0; y < columns; y++ ) {
			Serial.print(get(x,y));
			Serial.print(F("\t"));
		}
		Serial.print(F("\n"));
	}
#else
	std::cout << "Matrix :" << this->rows << " ";
	std::cout << this->columns;
	std::cout << std::endl;
	for (short x = 0; x<rows; x++) {
		for (short y = 0; y < columns; y++ ) {
			std::cout << get(x,y);
			std::cout << "\t";
		}
		std::cout << std::endl;
	}
#endif
}

#ifndef X86
    void Matrix::sendToSerial() const {
      Tuple shape = this->shape();
      Serial.write((char*) &(shape.t1),sizeof(int));
      Serial.write((char*) &(shape.t2),sizeof(int));
      Serial.write((char*) matrix,sizeof(float)*shape.t1*shape.t2);
    }


    Matrix Matrix::fromSerial(){
      int rows = 0;
      int columns = 0;
  
      BinarySerial::readInt(rows);
      BinarySerial::readInt(columns);
      //Serial.readBytes((char*) &rows,sizeof(int));
      //Serial.readBytes((char*) &columns,sizeof(int));
      float buffer[rows*columns];
      BinarySerial::nFloatsFromSerial(buffer,rows*columns);
  
      Matrix tmp (buffer,rows,columns);
      return tmp;
    }
#endif
