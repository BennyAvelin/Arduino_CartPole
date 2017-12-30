#include "Layer.h"
#include "Matrix.h"
#ifndef X86
#include<Arduino.h>
#else
#include<iostream>
#endif

Layer::Layer(){}

Layer::Layer(Layer & obj){
	this->weights = new Matrix(*obj.weights);
	this->bias = new Matrix(*obj.bias);
	this->activation = *activation;
	this->shape = weights->shape();
}

Layer::Layer(Matrix &weights, Matrix &bias, float (*activation)(float)){
	this->weights = new Matrix(weights);
	this->bias = new Matrix(bias);
	this->activation = *activation;
	this->shape = this->weights->shape();
}

Matrix Layer::transform(Matrix &input){
	return ((input*(*weights))+(*bias)).apply(activation);
}

Layer::~Layer(){
	delete weights;
	delete bias;
}

Matrix Layer::getWeights(){
	return Matrix(*weights);
}
Matrix Layer::getBias(){
	return Matrix(*bias);
}

float (*(Layer::getActivation()))(float){
	return activation;
}

void Layer::updateWeights(Matrix &newWeights, Matrix &newBias){
	delete weights;
	delete bias;
	weights = new Matrix(newWeights);
	bias = new Matrix(newBias);
}

Tuple Layer::getShape(){
	return shape;
}
