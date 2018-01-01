#define X86
#include "../Arduino/Bluetooth/Matrix.h"
#include "../Arduino/Bluetooth/Layer.h"
#include "../Arduino/Bluetooth/Network.h"
#include<iostream>

double &tomte(double &f){
	f = 2.0;
	return f;
}

float square(float x) {
	return x*x;
}

std::ostream &operator<<(std::ostream &os, Tuple const &m) { 
    return os << "(" << m.t1 << "," << m.t2 << ")";
}

struct Double {
	char b1;
	char b2;
	char b3;
	char b4;
};

int main() {
	float arr0[] = {1.0,1.0}; 
	float arr1[] = {1.0,1.0,2.3,4.2}; 
	float arr2[] = {3.5,2.0032}; 
	
	std::cout << "Making matrix" << std::endl;
	Matrix inputs = Matrix(arr0,1,2);
	std::cout << "Making matrix" << std::endl;
	Matrix weights = Matrix(arr1,2,2);
	std::cout << "Making matrix" << std::endl;
	Matrix bias = Matrix(arr2,1,2);
	std::cout << "Making layer" << std::endl;
	Layer layer (weights,bias,*Activations::relu);
	std::cout << "Making layer" << std::endl;
	Layer layer1 (weights,bias,*Activations::relu);
	std::cout << "Making network" << std::endl;
	Network network(2);
	std::cout << "Setting stage" << std::endl;
	network.addStage(layer);
	std::cout << "Setting stage" << std::endl;
	network.addStage(layer1);
	Matrix tmp = layer.transform(inputs);
	layer1.transform(tmp).print();
	Matrix output = network.transform(inputs);
	output.print();
	std::cout << sizeof(float*) << std::endl;
	std::cout << sizeof(int) << std::endl;
	std::cout << sizeof(Matrix) << std::endl;
	std::cout << sizeof(Layer) << std::endl;
	std::cout << sizeof(Network) << std::endl;
	//std::cout << {return "hello";} << std::endl;
	while(true){}
	//ans.print();
	//ans.transpose().print();
	/*buffer[0] = d.b1;
	buffer[1] = d.b2;
	buffer[2] = d.b3;
	buffer[3] = d.b4;*/
}