#ifndef MATRIX_H
#define MATRIX_H
#include "Tuple.h"

class Matrix {
    float *matrix;
	int rows;
	int columns;
	int totalSize;
  public:
	  Matrix();
	  Matrix( const Matrix &obj);
	  Matrix( float *input, int rows, int columns);
	  Matrix( int rows, int columns);
	  Matrix( int rows, int columns, float initialValue);
	  ~Matrix();
	  float &get(int row, int column); //Returns the value at row, column
	  //column for setting.
	  float &operator[](int index); //Gets a pointer to the location holding the
	  Matrix operator+(Matrix &B);
	  Matrix operator-(Matrix &B);
	  Matrix operator*(Matrix &B);
	  Matrix operator*(float scale);
	  Matrix pm(Matrix &B);
	  Matrix apply(float (*f)(float));
	  Matrix transpose(); //Returns a transpose of the matrix
	  Tuple shape();
	  //Matrix operator+(float num);
	  //Matrix operator-(float num);
	  void print();
};

#endif
