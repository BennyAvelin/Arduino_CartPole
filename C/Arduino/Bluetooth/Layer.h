#ifndef LAYER_H
#define LAYER_H
#include "Matrix.h"
#include "Tuple.h"

class Layer {
	Tuple shape;
	Matrix *weights;
	Matrix *bias;
	float (*activation)(float);
public:
	Layer();
	Layer(Matrix &weights, Matrix &bias, float (*activation)(float));
	Layer(Layer &obj);
	~Layer();
	Matrix transform(Matrix &input);
	Matrix getWeights();
	Matrix getBias();
	Tuple getShape();
	float (*(getActivation()))(float); //Function returning pointer to function taking a float and returning float.
	
	void updateWeights(Matrix &newWeights, Matrix &newBias);
};
#endif
