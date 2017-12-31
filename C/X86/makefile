CC = g++
CFLAGS = -c -DX86=1
CCFLAGS = -std=c++11

main: layermatrix
	$(CC) $(CCFLAGS) main.cc *.o
	
layermatrix: 
	$(CC) $(CCFLAGS) $(CFLAGS) ../Arduino/Bluetooth/Network.cpp ../Arduino/Bluetooth/Layer.cpp ../Arduino/Bluetooth/Matrix.cpp