#include "Network.h"
#ifndef X86
#include<Arduino.h>
#else
#include<iostream>
#endif

Network::Network(short numLayers){
	this->numLayers = numLayers;
	this->filledLayers = 0;
	/*layers = (Layer*) malloc(sizeof(Layer)*numLayers);
	for (short i=0; i < numLayers; i++){
		layers[i] = Layer();
	}*/
}

Network::~Network(){
	//free(layers);
}

Matrix Network::transform(Matrix &input){
	//poopy
	Matrix tmp;
	tmp = input;
	for (short i=0; i<numLayers; i++){
		tmp = layers[i].transform(tmp);
	}
	return tmp;
}

Layer &Network::operator[](short index){
	return layers[index];
}

void Network::addStage(Layer &stage){
	if (filledLayers < numLayers){
		layers[filledLayers++] = stage;
	}
}
