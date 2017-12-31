#include "Layer.h"
#include "Matrix.h"
#ifndef X86
#include<Arduino.h>
#else
#include<iostream>
#endif

float relu_func(float input) {
  return (input > 0) ? input : 0;
}

float linear_func(float input) {
  return input;
}

Layer::Layer(){
#ifdef X86
  std::cout << "Layer  constructed empty" << std::endl;
#endif
}

Layer::Layer(Layer & obj){
#ifdef X86
  std::cout << "Layer  copied" << std::endl;
#endif
	this->weights = obj.weights;
	this->bias = obj.bias;
	this->activation = *obj.activation;
	this->shape = weights.shape();
}

Layer::Layer(Matrix &weights, Matrix &bias, float (*activation)(float)){
#ifdef X86
  std::cout << "Layer  being specified" << std::endl;
#endif
	this->weights = weights;
	this->bias = bias;
	this->activation = *activation;
	this->shape = this->weights.shape();
}

Matrix Layer::transform(Matrix &input){
	return ((input*(weights))+(bias)).apply(activation);
}

Layer::~Layer(){
}

Layer& Layer::operator=(Layer obj){
#ifdef X86
  std::cout << "Layer  copied by equals" << std::endl;
#endif
  this->weights = obj.weights;
  this->bias = obj.bias;
  this->activation = *obj.activation;
  this->shape = weights.shape();
  return *this;
}

Matrix Layer::getWeights(){
	return Matrix(weights);
}
Matrix Layer::getBias(){
	return Matrix(bias);
}

float (*(Layer::getActivation()))(float){
	return activation;
}

void Layer::updateWeights(Matrix &newWeights, Matrix &newBias){
	weights = newWeights;
	bias = newBias;
}

Tuple Layer::getShape(){
	return shape;
}
