#ifndef BINARYSERIAL_H
#define BINARYSERIAL_H

class BinarySerial {
  public:
    inline static void getFloatFromSerial(float &buffer){
      char *cBuffer = (char*) &buffer;
      Serial.readBytes(cBuffer,sizeof(float));
      //if (Serial.readBytes(cBuffer,sizeof(float)) != 4) digitalWrite(LED_BUILTIN, HIGH);
    }

    inline static void nFloatsFromSerial(float *array, int n){
    	for (int i=0; i<n; i++) {
        	getFloatFromSerial(array[i]);
    	}
	}
	
	inline static void readInt(int &buffer){
		Serial.readBytes((char*) &buffer,sizeof(int));
	}
};

#endif