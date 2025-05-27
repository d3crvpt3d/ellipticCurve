#include <stdio.h>

typedef unsigned char bool;
typedef unsigned long long int uint64_t;

#define MSB_MASK_64 ((uint64_t)1 << 63) 

//LSB
typedef struct {
	uint64_t digits[8];
} uint512_t;

typedef struct{
	uint512_t p;
	uint512_t a;
	uint512_t b;
	uint512_t n;
} Curve_t;

typedef struct{
	bool just;

	struct{
		uint512_t x;
		uint512_t y;
	};
} Point_t;

//uint512_t -> uint512_t
uint512_t fe_mod(uint512_t a, uint512_t n){
	uint512_t *tmp = (uint512_t *) &a;
	return *tmp; //TODO
}

uint512_t fe_add(uint512_t a, uint512_t b){
	
	bool carry = 0;
	uint512_t output = {0}; //initialize empty output
	
	//beginning from LSByte, add and store carry for next byte
	for (unsigned char i = 0; i < 8; i++) {
        uint64_t temp;

        // Add a + b, check for carry
        bool overflow1 = __builtin_add_overflow(a.digits[i], b.digits[i], &temp);

        // Add carry if needed
        bool overflow2 = __builtin_add_overflow(temp, carry, &output.digits[i]);

        // Update carry: if either add overflowed, we carry to next limb
        carry = overflow1 || overflow2;
    }

	return output;
}



//[1010,1010,1010,1010] -> [0101,0101,0101,0100]
uint512_t fe_half(uint512_t x){
	//TODO: 8 wide limbs fix
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

uint512_t fe_inverse(uint512_t x){
	return x; //TODO
}

uint512_t fe_double(uint512_t x){
	
	uint64_t carrys[7] = {
		(x.digits[0] >> 63) & 1,
		(x.digits[1] >> 63) & 1,
		(x.digits[2] >> 63) & 1,
		(x.digits[3] >> 63) & 1,
		(x.digits[4] >> 63) & 1,
		(x.digits[5] >> 63) & 1,
		(x.digits[6] >> 63) & 1,
	};

	x.digits[0] <<= 1; //skip first, bc we dont neet to apply carry
	
	for(unsigned char i = 1; i < 8; i++){
		x.digits[i] = (x.digits[i] << 1) | carrys[i-1];
	}
	
	return x;
}

//a = a*b
//fast exp like
uint512_t fe_mult(uint512_t a, uint512_t b){

	uint512_t x = {0};
	unsigned char first = 1;

	//iterate over each integer
	for(unsigned char limb = 7; limb < 0xFF; limb--){
		//iterate over each bit from MSD to LSD
		for(unsigned char bit = 63; bit < 0xFF; bit--){ //wanted integer underflow

			x = fe_double(x);

			if((b.digits[limb] >> bit) & 1){
				x = fe_add(x, a);
			}
		}
	}

	return x; //TODO: check
}

//x = x^2
uint512_t fe_square(uint512_t x){
	return fe_mult(x, x);
}

//fast pow with implicit modulus
uint512_t fe_pow(uint512_t b, uint64_t x){
	
	//if <2; return
	if(x < 2){
		return b;
	}

	uint512_t acc = b;
	
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
uint512_t curve(uint512_t x, bool odd, Curve_t curve){
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
			curve( (uint512_t) {0x79BE667EF9DCBBAC, 0x55A06295CE870B07, 0x029BFCDB2DCE28D9, 0x59F2815B16F81798}, 0, secp256k1)}
	};

	//DEBUG
	g.x = (uint512_t) {0xFFFFFFFFFFFFFFFF,0xFFFFFFFF, 0, 0, 0, 0, 0, 0};
	printf("%016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx\n", g.x.digits[7], g.x.digits[6], g.x.digits[5], g.x.digits[4], g.x.digits[3], g.x.digits[2], g.x.digits[1], g.x.digits[0]);
	g.x = fe_mult(g.x, g.x);
	printf("%016llx%016llx%016llx%016llx%016llx%016llx%016llx%016llx\n", g.x.digits[7], g.x.digits[6], g.x.digits[5], g.x.digits[4], g.x.digits[3], g.x.digits[2], g.x.digits[1], g.x.digits[0]);
	return 0;
}




































































































