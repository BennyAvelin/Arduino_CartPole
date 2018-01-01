#include "Layer.h"
#include "Matrix.h"
#ifndef X86
#include<Arduino.h>
#else
#include<iostream>
#endif

float Activations::relu(float input) {
  return (input > 0) ? input : 0;
}

float Activations::linear(float input) {
  return input;
}

Layer::Layer(){
#ifdef X86
  std::cout << "Layer  constructed empty" << std::endl;
#endif
}

Layer::Layer(const Layer & obj){
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

Matrix Layer::transform(Matrix &input) const{
	return ((input*(weights))+(bias)).apply(activation);
}

Layer::~Layer(){
}

Layer& Layer::operator=(const Layer &obj){
#ifdef X86
  std::cout << "Layer  copied by equals" << std::endl;
#endif
  this->weights = obj.weights;
  this->bias = obj.bias;
  this->activation = *obj.activation;
  this->shape = weights.shape();
  return *this;
}

Matrix Layer::getWeights() const{
	return Matrix(weights);
}
Matrix Layer::getBias() const{
	return Matrix(bias);
}

float (*(Layer::getActivation()))(float){
	return activation;
}

void Layer::updateWeights(Matrix &newWeights, Matrix &newBias){
	weights = newWeights;
	bias = newBias;
}

Tuple Layer::getShape() const{
	return shape;
}

Layer Layer::fromSerial() {
  Matrix weights = Matrix::fromSerial();
  Serial.println(F("Recieved weights"));
  Matrix bias = Matrix::fromSerial();
  Serial.println(F("Recieved bias"));
  Layer lay (weights,bias,*Activations::relu);
  return lay;
}

