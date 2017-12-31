/*
* Recieve a neural network over serial connection
* Coder - Benny Avelin
* Website - 
*/
#include "Matrix.h"
#include "Layer.h"
#include "Tuple.h"
#include "Network.h"

char data = 0;            //Variable for storing received data


void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
}

void outOfMemoryError() {
  Serial.println(F("Out of memory"));
}

float squares(float x){
  return x*x;
}

void getFloatFromSerial(float &buffer){
  char *cBuffer = (char*) &buffer;
  Serial.readBytes(cBuffer,sizeof(float));
}

void nFloatsFromSerial(float *array, int n){
  for (int i=0; i<n; i++) {
      getFloatFromSerial(array[i]);
  }
}

Matrix readMatrixFromSerial() {
  int rows = 0;
  int columns = 0;
  
  Serial.readBytes((char*) &rows,sizeof(int));
  Serial.readBytes((char*) &columns,sizeof(int));
  
  float buffer[rows*columns];
  nFloatsFromSerial(buffer,rows*columns);
  
  Matrix tmp (buffer,rows,columns);
  return tmp;
}

void sendMatrixToSerial(Matrix &mat) {
  Tuple shape = mat.shape();
  Serial.write((char*) &(shape.t1),sizeof(int));
  Serial.write((char*) &(shape.t2),sizeof(int));
  Serial.write((char*) mat.getRawData(),sizeof(float)*shape.t1*shape.t2);
}

void loop()
{
  /*float arr0[] = {1.0,1.0,2.3}; 
  float arr1[] = {1.0,1.0,2.3,4.2,5.3,7.2}; 
  float arr2[] = {3.5,2.0032}; 
  
  Matrix inputs = Matrix(arr0,1,3);
  Matrix weights = Matrix(arr1,3,2);
  Matrix bias = Matrix(arr2,2,1);
  Layer layer (weights,bias,*Activations.relu);
  layer.getWeights().print();*/
  if (Serial.available() > 0) {
    //Read weight matrix
    Matrix weights = readMatrixFromSerial();
    //Read bias matrix
    Matrix bias = readMatrixFromSerial();
    //Setup layer
    Layer layer (weights,bias,*Activations.relu);
    Matrix weights1 = readMatrixFromSerial();
    //Read bias matrix
    Matrix bias1 = readMatrixFromSerial();
    //Setup layer
    Layer layer1 (weights1,bias1,*Activations.linear);

    Network network(2);
    *network[0] = layer;
    *network[1] = layer1;
    //Read input
    Matrix input = readMatrixFromSerial();
    //Transform input
    Matrix output = network.transform(input);
    //Send transformed input
    sendMatrixToSerial(output);
  }
}
