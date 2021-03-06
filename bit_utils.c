#include <stdio.h>
#include "bit_utils.h"

int neg_value(int a) {
	return ~a + 1;
}

int count_one(unsigned long a) {
	int n = 0;
	while(a) {
		if(a & 0x01) n++;
		a >>= 1;
	}
	return n;
}

long swap_bits(long x, int a, int b) {
	if((x & (0x01L<<a)) != (x & (0x01L<<b))) {
		x ^= 0x01L<<a;
		x ^= 0x01L<<b;
	}
	return x;
}

void print_bits(unsigned long x) {
	int w = sizeof(x) * 8;
	printf("0b");
	while(w) {
		if(x & (0x01UL<<(w-1))) printf("1");
		else printf("0");
		w--;
	}
}


