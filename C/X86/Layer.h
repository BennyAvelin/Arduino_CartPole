#ifndef LAYER_H
#define LAYER_H
#include "Matrix.h"
#include "Tuple.h"

class Layer {
	Tuple shape;
	Matrix *weights;
	Matrix *bias;
	double (*activation)(double);
public:
	Layer();
	Layer(Matrix &weights, Matrix &bias, double (*activation)(double));
	Layer(Layer &obj);
	~Layer();
	Matrix transform(Matrix &input);
	Matrix getWeights();
	Matrix getBias();
	Tuple getShape();
	double (*(getActivation()))(double); //Function returning pointer to function taking a double and returning double.
	
	void updateWeights(Matrix &newWeights, Matrix &newBias);
};
#endif