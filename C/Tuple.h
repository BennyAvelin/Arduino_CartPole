#ifndef TUPLE_H
#define TUPLE_H
struct Tuple {
	int t1;
	int t2;
	inline Tuple(int a, int b){
		t1 = a;
		t2 = b;
	}
	inline Tuple(){
		t1 = t2 = 0;
	}
};
#endif