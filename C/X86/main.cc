#define X86
#include "../Arduino/Bluetooth/Matrix.h"
#include "../Arduino/Bluetooth/Layer.h"
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
	int sizex = 2;
	int sizey = 2;
	float arr0[] = {1.0,1.0,2.3}; 
	float arr1[] = {1.0,1.0,2.3,4.2,5.3,7.2}; 
	float arr2[] = {3.5,2.0032}; 
	
	Matrix inputs = Matrix(arr0,1,3);
	Matrix weights = Matrix(arr1,3,2);
	Matrix bias = Matrix(arr2,2,1);
	{
		Layer layer (weights,bias,*square);
		std::cout << layer.getShape() << std::endl;
		std::cout << layer.getShape() << std::endl;
	}
	//ans.print();
	//ans.transpose().print();
	while (true){}
	/*buffer[0] = d.b1;
	buffer[1] = d.b2;
	buffer[2] = d.b3;
	buffer[3] = d.b4;*/
}