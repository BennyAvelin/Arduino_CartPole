#ifndef NETWORK_H
#define NETWORK_H
#include "Layer.h"
#define MAX_LAYERS 2

class Network {
	Layer layers[MAX_LAYERS];
	short numLayers;
	short filledLayers;
public:
	Network(short numLayers);
	~Network();
	Matrix transform(Matrix &input);
	Layer& operator[](short index);
	void addStage(Layer &stage);
};

#endif