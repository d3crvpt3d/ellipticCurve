#include <stdio.h>

typedef unsigned char bool;
typedef unsigned long long int uint64_t;

//LSB
typedef struct {
	uint64_t digits[4];
} uint256_t;

typedef struct{
	uint256_t p;
	uint256_t a;
	uint256_t b;
	uint256_t n;
} Curve_t;

typedef struct{
	bool just;

	struct{
		uint256_t x;
		uint256_t y;
	};
} Point_t;


uint256_t fe_add(uint256_t a, uint256_t b){
	
	uint64_t carry = 0;
	uint256_t output = {0};
	for(unsigned char i = 0; i < 4; i++){
		output.digits[i] = a.digits[i]+b.digits[i]+carry;
		carry = (a.digits[i] << 63) && (b.digits[i] << 63);
	}

	return output;
}

//[1010,1010,1010,1010] -> [0101,0101,0101,0100]
uint256_t fe_half(uint256_t x){

	uint64_t carrys[3] = {x.digits[1] & 1, x.digits[2] & 1, x.digits[3] & 1};

	//[001,100,101,001] << 1 => [011,001,010,010]

	x.digits[0] = x.digits[0] >> 1;
	x.digits[1] = x.digits[1] >> 1;
	x.digits[2] = x.digits[2] >> 1;
	x.digits[3] = x.digits[3] >> 1;
	
	x.digits[0] |= carrys[0] << 63;
	x.digits[1] |= carrys[1] << 63;
	x.digits[2] |= carrys[2] << 63;

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
uint256_t fe_pow(uint256_t b, uint64_t x){
	
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
	if(odd){
		return x;
	}else{
		return x;
	}

}

int main(){

	Curve_t secp256k1 = {
		{{0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFEFFFFFC2F}},
		{{0,0,0,0}},
		{{0,0,0,7}},
		{0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE, 0xBAAEDCE6AF48A03B, 0xBFD25E8CD0364141}
	};
	
	Point_t g = {
		1,
		{
			{0x79BE667EF9DCBBAC, 0x55A06295CE870B07, 0x029BFCDB2DCE28D9, 0x59F2815B16F81798},
			curve( (uint256_t) {0x79BE667EF9DCBBAC, 0x55A06295CE870B07, 0x029BFCDB2DCE28D9, 0x59F2815B16F81798}, 0, secp256k1)}
	};

	//DEBUG
	g.x = fe_add(g.x, g.x);
	printf("%016llx%016llx%016llx%016llx\n", g.x.digits[0], g.x.digits[1], g.x.digits[2], g.x.digits[3]);
	return 0;
}




































































































