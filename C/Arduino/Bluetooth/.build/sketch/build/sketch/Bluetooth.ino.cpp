#include <Arduino.h>
#line 1 "/Users/avelin/python/Arduino_CartPole/C/Arduino/Bluetooth/Bluetooth.ino"
#line 1 "/Users/avelin/python/Arduino_CartPole/C/Arduino/Bluetooth/Bluetooth.ino"
/*
* Recieve a neural network over serial connection
* Coder - Benny Avelin
* Website - 
*/
#include "Matrix.h"
#include "Layer.h"
#include "Tuple.h"
#include "Network.h"
#include<Time.h>

char data = 0;            //Variable for storing received data

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory() {
  int free_memory;

  if((int)__brkval == 0)
     free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}

void setup()
{
    Serial.begin(9600);   //Sets the baud for serial data transmission                               
    Serial.setTimeout(2000);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (Serial.available() > 0) {
    Serial.println(freeMemory());
    Network net = Network::fromSerial(2);
    Matrix inputs = Matrix::fromSerial();
    Matrix output = net.transform(inputs);
    Serial.println(F("Recieved weights"));
    Serial.println(freeMemory());
    unsigned long time = millis();
    for (int i=0; i<1000; i++) {
      //Transform input
      Matrix output1 = net.transform(inputs);
    }
    Serial.println(millis()-time);
    Serial.println(freeMemory());

    
    /*//Read weight matrix
    Matrix weights = readMatrixFromSerial();
    Serial.println("Recieved weights");
    //Read bias matrix
    Matrix bias = readMatrixFromSerial();
    Serial.println("Recieved bias");
    //Setup layer
    Layer layer (weights,bias,*Activations.relu);
    Matrix weights1 = readMatrixFromSerial();
    Serial.print("Recieved weights1");
    Serial.print(weights1.shape().t1);
    Serial.print(" ");
    Serial.print(weights1.shape().t2);
    Serial.println(" Thats it");
    //Read bias matrix
    Matrix bias1 = readMatrixFromSerial();
    Serial.println("Recieved bias1");
    //Setup layer
    Layer layer1 (weights1,bias1,*Activations.linear);

    Network network(2);
    network.addStage(layer);
    network.addStage(layer1);
    //Read input
    Matrix input = readMatrixFromSerial();
    Serial.println("Recieved input");
    unsigned long time = millis();
    for (int i=0; i<1000; i++) {
      //Transform input
      Matrix output = network.transform(input);
    }
    Serial.println(millis()-time);
    Serial.println(freeMemory());*/
  }
}

