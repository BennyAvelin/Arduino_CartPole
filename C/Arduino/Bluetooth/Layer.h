#ifndef LAYER_H
#define LAYER_H
#include "Matrix.h"
#include "Tuple.h"

class Layer {
	Tuple shape;
	Matrix weights;
	Matrix bias;
	float (*activation)(float);
public:
	//----------------Constructors and Destructors-----------------------
	Layer();
	Layer(Matrix &weights, Matrix &bias, float (*activation)(float));
	Layer(const Layer &obj);
	~Layer();
	//----------------Operators -----------------------------------------
	Layer& operator=(const Layer &B);
	//----------------Basic operations-----------------------------------
	/**
	* Transforms the input matrix by performing the computation
	* activation(input*weight+bias) and returning the result as a matrix.
	*/
	Matrix transform(Matrix &input) const;
	/**
	* Getters for the weights and bias
	*/
	Matrix getWeights() const;
	Matrix getBias() const;
	Tuple getShape() const;
	/*
	 * Function returning pointer to function 
	 * taking a float and returning float.
	 */
	float (*(getActivation()))(float);
	/*
	 * Update the weights for this Layer.
	 */
	void updateWeights(Matrix &newWeights, Matrix &newBias);
	/*
	 * Serial stuff for the Arduino
	 */
#ifndef X86
	static Layer fromSerial();
#endif
};

class Activations {
  public:
  static float relu(float input);
  static float linear(float input);
};

#endif
