#ifndef NETWORK_H
#define NETWORK_H
#include "Layer.h"
#define MAX_LAYERS 2

class Network {
	Layer layers[MAX_LAYERS];
	short numLayers;
	short filledLayers;
public:
  //----------------Constructors and Destructors-----------------------
	Network(short numLayers);
	~Network();
  //----------------General methods------------------------------------
  /**
   * Applies each layers transformation in sucession, i.e.
   * layers[1].transform(layers[0].transform(input)) and so on 
   * for more layers.
   */
	Matrix transform(Matrix &input);
  /**
   * Returns a reference to a specific layer
   */
	Layer& operator[](short index);
  /**
   * The actual method to add a Layer to the network. Beware that the layer will be copied
   * and there has to be room for that in the memory, otherwise odd things will start to happen.
   */
	void addStage(const Layer &stage);
  static Network fromSerial(int n);
};

#endif
