/*
* Recieve a neural network over serial connection
* Coder - Benny Avelin
* Website - 
*/
#include "Matrix.h"
#include "Layer.h"

char data = 0;            //Variable for storing received data


void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    Serial.print(F("Send each layer as 'row col', followed by row by row of data\n"));
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

void loop()
{
  float arr0[] = {1.0,1.0,2.3}; 
  float arr1[] = {1.0,1.0,2.3,4.2,5.3,7.2}; 
  float arr2[] = {3.5,2.0032}; 
  
  Matrix inputs = Matrix(arr0,1,3);
  Matrix weights = Matrix(arr1,3,2);
  Matrix bias = Matrix(arr2,2,1);
  Layer layer (weights,bias,*squares);
  layer.getWeights().print();
}
