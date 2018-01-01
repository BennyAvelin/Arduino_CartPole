#ifndef MATRIX_H
#define MATRIX_H
#include "Tuple.h"

class Matrix {
    float *matrix;
	short rows;
	short columns;
	short totalSize;
  public:
  //----------------Constructors and Destructors-----------------------
	  Matrix();
	  Matrix( const Matrix &obj);
	  Matrix( float *input, short rows, short columns);
	  Matrix( short rows, short columns);
	  Matrix( short rows, short columns, float initialValue);
	  ~Matrix();
  //----------------Setters and getters--------------------------------
	  float &set(short row, short column); //Returns the value at row, column
    float get(short row, short column) const; //Returns the value at row, column
	  //column for setting.
	  float &operator[](short index); //Gets a poshorter to the location holding the
  //----------------Operators on Matrices------------------------------
  /**
   * Each operator returns a new Matrix as the result, which is the only thing that
   * makes sense. This is because the returned element will have a different shape,
   * so we end up freeing and setting up new memory anyway.
   */
	  Matrix operator+(const Matrix &B) const;
	  Matrix operator-(const Matrix &B) const;
	  Matrix operator*(const Matrix &B) const;
	  Matrix operator*(const float scale) const;
	  Matrix& operator=(const Matrix &B);
	  Matrix pm(const Matrix &B) const;
   /**
    * Takes a pointer to function taking float and returning float, and applies
    * it to every element in the matrix.
    */
	  Matrix apply(float (*f)(float)) const;
    /**
     * Returns a transposed copy of the matrix.
     */
	  Matrix transpose() const; //Returns a transpose of the matrix
    /**
     * Returns the shape of the matrix as a Tuple object, t1=rows,t2=cols.
     */
	  Tuple shape() const;
    #ifndef X86
    /**
     * Sends the matrix to Arduino Serial connection.
     * The first two bytes are the number of rows, the second two are columns,
     * thereafter the contents of the matrix follows each element taking 4 bytes
     * of space.
     */
    void sendToSerial() const;
    /**
     * Global class method to recieve a matrix from the Arduino Serial connection
     * according to the System mentioned above.
     */
    static Matrix fromSerial();
    #endif
	  void print() const;
   private:
    /**
     * Returns the raw matrix-data as an array of float.
     */
   float *getRawData();
};

#endif
