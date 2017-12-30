#ifndef MATRIX_H
#define MATRIX_H
#include "Tuple.h"

class Matrix {
    double *matrix;
	int rows;
	int columns;
	int totalSize;
  public:
	  Matrix();
	  Matrix( const Matrix &obj);
	  Matrix( double *input, int rows, int columns);
	  Matrix( int rows, int columns);
	  Matrix( int rows, int columns, double initialValue);
	  ~Matrix();
	  double &get(int row, int column); //Returns the value at row, column
	  //column for setting.
	  double &operator[](int index); //Gets a pointer to the location holding the
	  Matrix operator+(Matrix &B);
	  Matrix operator-(Matrix &B);
	  Matrix operator*(Matrix &B);
	  Matrix operator*(double scale);
	  Matrix pm(Matrix &B);
	  Matrix apply(double (*f)(double));
	  Matrix transpose(); //Returns a transpose of the matrix
	  Tuple shape();
	  //Matrix operator+(double num);
	  //Matrix operator-(double num);
	  void print();
};

#endif