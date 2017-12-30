CC = g++
CFLAGS = -c -DX86=1

main: layermatrix
	$(CC) main.cc *.o
	
layermatrix: 
	$(CC) $(CFLAGS) ../Arduino/Bluetooth/Layer.cpp ../Arduino/Bluetooth/Matrix.cpp