/*
 * mp_arith.c
 *
 */
#include <stdint.h>
#include <stdio.h>


// Calculates res = a + b.
// a and b represent large integers stored in uint32_t arrays
// The first index of the array is the length of the array
// res should be an array of length = min(sizeA, sizeB) + 1
void mp_add(uint32_t *a, uint32_t *b, uint32_t *res)
{
	uint32_t sizeA = a[0];
	uint32_t sizeB = b[0];
	uint32_t minSize;
	if (sizeA > sizeB) {
		minSize = sizeB;
	} else {
		minSize = sizeA;
	}
	
	uint32_t c = 0;
	uint64_t temp;
	int i;
	for(i=1; i<minSize; i++) {
		temp = (uint64_t)a[i] + (uint64_t)b[i] + (uint64_t)c;
		res[i] = (uint32_t)temp;
		c = temp >> 32;
	}
	res[minSize] = c;	// the memory at res[size]
}

// Calculates res = a - b.
// a and b represent large integers stored in uint32_t arrays
// a, b and res are arrays of size elements
void mp_sub(uint32_t *a, uint32_t *b, uint32_t *res)
{	
	uint32_t sizeA = a[0];
	uint32_t sizeB = b[0];
	uint32_t minSize;
	if (sizeA > sizeB) {
		minSize = sizeB;
	} else {
		minSize = sizeA;
	}
	
	uint32_t c = 0;
	uint64_t temp;
	int i;
	for(i=1; i<minSize; i++) {
		temp = (uint64_t)a[i] - (uint64_t)b[i] - (uint64_t)c;
		res[i] = (uint32_t)temp;
		c = temp >> 63; // c = 000...001 or 000...000
	}
}





