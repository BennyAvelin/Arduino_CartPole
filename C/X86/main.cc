#include "Matrix.h"
#include "Layer.h"
#include<iostream>

double &tomte(double &f){
	f = 2.0;
	return f;
}

double square(double x) {
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
	/*int sizex = 2;
	int sizey = 2;
	double arr0[] = {1.0,1.0,2.3}; 
	double arr1[] = {1.0,1.0,2.3,4.2,5.3,7.2}; 
	double arr2[] = {3.5,2.0032}; 
	
	Matrix inputs = Matrix(arr0,1,3);
	Matrix weights = Matrix(arr1,3,2);
	Matrix bias = Matrix(arr2,2,1);
	{
		Layer layer (weights,bias,*square);
		std::cout << layer.getShape() << std::endl;
	}
	//ans.print();
	//ans.transpose().print();
	while (true){}*/
	char *buffer;
	buffer = (char*) malloc(4);
	float tomte = 2.3;
	char *d = (char*) &tomte;
	for (int i=0; i<4; i++){
		std::cout << (int) d[i] << " ";
	}
	float tomte2 = *((float*) d);
	std::cout << tomte2 << std::endl;
	/*buffer[0] = d.b1;
	buffer[1] = d.b2;
	buffer[2] = d.b3;
	buffer[3] = d.b4;*/
}