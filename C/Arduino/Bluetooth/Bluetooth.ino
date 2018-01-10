/*
* Recieve a neural network over serial connection
* Coder - Benny Avelin
* Website - 
*/
#include "Matrix.h"
#include "Layer.h"
#include "Tuple.h"
#include "Network.h"
#include "HotStepper.h"
#include<Time.h>
#define TEST

HotStepper stepper1(&PORTB, 0b00001111);

char data = 0;            //Variable for storing received data

const int xpin = A2;                  // x-axis of the accelerometer
const int ypin = A1;                  // y-axis
const int zpin = A0;                  // z-axis (only on 3-axis models)
float currentAngle = 0.0;
int dir = 0;
#define WINDOW 5
#define smoothingVal 1
#define SMOOTH(x) (smoothingVal*(((int) x)/smoothingVal))
float windowx[WINDOW];
float windowy[WINDOW];
float windowz[WINDOW];

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

#define AVGSTEPS 1
#define DEG90 64.5*AVGSTEPS

int pos = 0;

void averagedAcc(float *arr) {
  windowx[pos] = ((float) analogRead(xpin))/WINDOW;
  windowy[pos] = ((float) analogRead(ypin))/WINDOW;
  windowz[pos] = ((float) analogRead(zpin))/WINDOW;
  
  for (int i = 0; i<3; i++){
    arr[i] = 0;
  }
  for (int i=0; i<WINDOW; i++) {
    arr[0] += windowx[i];
    arr[1] += windowy[i];
    arr[2] += windowz[i];
  }
  pos = (pos+1) % WINDOW;
}

void computeState(float *state) {
  #define SPR 2048.0
  #define R 0.033
  float pos = R*stepper1.currentPos()/SPR;
  state[0] = pos;
  float arr[3];
  averagedAcc(arr);
  averagedAcc(arr);
  state[1] = SMOOTH(arr[0]);
  state[2] = SMOOTH(arr[1]);
  state[3] = SMOOTH(arr[2]);
}

void sendState(const float *state) {
  Serial.write((char*) &state[0],sizeof(float));
  Serial.write((char*) &state[1],sizeof(float));
  Serial.write((char*) &state[2],sizeof(float));
  Serial.write((char*) &state[3],sizeof(float));
  //Serial.print(state[0]);
  //Serial.print("\t");
  //Serial.println(state[1]);
}

void setup()
{
    Serial.begin(115200);   //Sets the baud for serial data transmission                               
    Serial.setTimeout(100000);
    pinMode(LED_BUILTIN, OUTPUT);
    int arr[3];
    HotStepper::setup();
    stepper1.turn(1,0);
}

float position = 0;

void loop()
{
#ifdef TEST
  delay(2);
  float value[3];
  stepper1.continuousTurn(dir = !dir);
  for (int i=0; i<1; i++) {
    averagedAcc(value);
  }
  Serial.print(value[2]);
  /*// print a tab between values:
  Serial.print("\t");
  Serial.print((value[1]+value[0]));
  // print a tab between values:
  Serial.print("\t");
  Serial.print(((value[2]+value[0])));*/
  Serial.println("");
  delay(20);
#else
  float state[4];
  computeState(state);
  sendState(state);
  int action = Serial.parseInt();
  if (action == -1) {
    //This implies we should reset
    //Serial.println(stepper1.currentPos());
    stepper1.returnToZero();
    while(!stepper1.ready()) {}
    while (Serial.parseInt() == -1) {}
    //Here we should make a read of the sensor values to relase the stored values
    float arr[3];
    for (int i=0; i<WINDOW; i++){
      averagedAcc(arr);
    }
  } else {
    stepper1.continuousTurn(action);
  }
#endif
  /*if (Serial.available() > 0) {
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
    Serial.println(freeMemory());*/

    
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
