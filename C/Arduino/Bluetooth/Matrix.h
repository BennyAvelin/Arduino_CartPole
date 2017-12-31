#ifndef MATRIX_H
#define MATRIX_H
#include "Tuple.h"

class Matrix {
    float *matrix;
	short rows;
	short columns;
	short totalSize;
  public:
	  Matrix();
	  Matrix( const Matrix &obj);
	  Matrix( float *input, short rows, short columns);
	  Matrix( short rows, short columns);
	  Matrix( short rows, short columns, float initialValue);
	  ~Matrix();
	  float &get(short row, short column); //Returns the value at row, column
	  //column for setting.
	  float &operator[](short index); //Gets a poshorter to the location holding the
	  Matrix operator+(Matrix &B);
	  Matrix operator-(Matrix &B);
	  Matrix operator*(Matrix &B);
	  Matrix operator*(float scale);
	  Matrix& operator=(const Matrix &B);
	  Matrix pm(Matrix &B);
	  Matrix apply(float (*f)(float));
	  Matrix transpose(); //Returns a transpose of the matrix
	  Tuple shape();
	  float *getRawData();
    #ifndef X86
    static Matrix fromSerial();
    #endif
	  //Matrix operator+(float num);
	  //Matrix operator-(float num);
	  void print();
};

#endif
