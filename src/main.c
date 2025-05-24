#include <stdint.h>
typedef unsigned char bool;

//LSB
typedef struct {
	uint64_t digits[4];
} uint256_t;

typedef struct{
	uint64_t p;
	uint64_t a;
	uint64_t b;
} Curve_t;

typedef struct{
	bool just;

	struct{
		uint256_t x;
		uint256_t y;
	};
} Point_t;


uint256_t fe_add(uint256_t ints, ...){
	//TODO
	return (uint256_t) {0,0,0,0};
}

//[1010,1010,1010,1010] -> [0101,0101,0101,0100]
uint256_t fe_half(uint256_t x){

	bool carrys[3] = {x.digits[1] << 63, x.digits[2] << 63, x.digits[3] << 63};

	x.digits[0] = x.digits[0] >> 1; //shift left
	x.digits[0] &= carrys[0]; //set msb of limb

	return x;
}

uint256_t fe_inverse(uint256_t x){
	return x; //TODO
}

//a = a*b
//fast exp like
uint256_t fe_mult(uint256_t a, uint256_t b){
	return a; //TODO
}

//x = x^2
uint256_t fe_square(uint256_t x){
	return fe_mult(x, x);
}

//fast pow with implicit modulus
uint256_t fe_pow(uint256_t b, uint32_t x){
	
	//if <2; return
	if(x < 2){
		return b;
	}

	uint256_t acc = b;
	
	while(x != 0){
		x = x >> 1;
		//if odd acc^2*b, if even acc^2
		if(x >> 31){
			fe_square(acc);
			fe_mult(acc, b); 
		}else{
			fe_square(acc);
		}
	}
	return acc;
}

//calculate y from x
uint256_t curve(uint256_t x, bool odd, Curve_t curve){
	return sqrt(add(pow(x,3), mul(a,x), b); //y = +-sqrt(x^3+ax+b)
}

int main(){

	Curve_t ed25519 = {486662, 1};
	Point_t g = {1, { , curve()}};


	return 0;
}




































































































