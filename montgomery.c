/*
 * montgomery.c
 *
 */

#include "montgomery.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void mp_sub(uint32_t *a, uint32_t *b, uint32_t *res, uint32_t size)
{
	uint32_t c = 0;
	uint32_t i = 0;

	for(i = 0; i<size; i++){
		res[i] = (a[i]-b[i]+c);
		if (a[i]>=b[i]-c){
			c = 0;
		}
		else{
			c = -1;
		}
	}
}

bool larger_equal_than(uint32_t *a, uint32_t *b,uint32_t size_a,uint32_t size_b){
	uint32_t counter;
	if (size_a > size_b){
		uint32_t difference = size_a-size_b;
		for (counter = 0; counter < difference; counter++){
			if (a[size_a - counter -1] > 0){
				return true;
			}
		}
		uint32_t i = size_b-1;
		while (i>0){
			if (a[i-difference]<b[i]){
				return false;
			}
			else if (a[i-difference]>b[i]){
				return true;
			}
			else{
				i--;
			}
		}
		return true;

	}
	else{
		uint32_t difference = size_b-size_a;
		for (counter = 0; counter < difference; counter++){
			if (b[size_b - counter -1] > 0){
				return false;
			}
		}
		uint32_t i = size_a-1;
		while (i>0){
			if (a[i]<b[i-difference]){
 				return false;
			}
			else if (a[i]>b[i-difference]){
				return true;
			}
			else{
				i--;
			}
		}
		return true;
	}
}

void mont(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *n0, uint32_t *res, uint32_t SIZE){
	uint32_t t[2*SIZE + 1];
	uint32_t i;
	for (i = 0; i<2*SIZE;i++){
		t[i] = 0;
	}
	uint32_t j;
	uint32_t S = 0;
	for (i = 0; i < SIZE; i++){
		uint32_t C = 0;
		for (j = 0; j < SIZE; j++){
			uint64_t sum = (uint64_t)t[i + j] + (uint64_t)a[j] * (uint64_t)b[i] + (uint64_t)C;
			S = (uint32_t)sum;
			C = (uint32_t)(sum >> 32);
			t[i+j] = S;
		}
		t[i+SIZE] = C;
	}
	for (i = 0; i < SIZE; i++){
		uint32_t C = 0;
		uint32_t m = (uint32_t)(t[i] * (uint32_t)(n0[0]));
		for (j = 0; j < SIZE; j++){
			uint64_t sum = (uint64_t)t[i + j] + (uint64_t)m * (uint64_t)n[j] + (uint64_t)C;
			S = (uint32_t)sum;
			C = (uint32_t)(sum >> 32);
			t[i+j] = S;
		}
		ADD(t,i+SIZE,C);
	}
	for (j=0; j<SIZE;j++){
		res[j]=t[j+SIZE];
	}
	SUB_COND(res,n,SIZE);
}

void ADD(uint32_t *t, uint32_t i, uint32_t C) {
	uint32_t S = 0;
	uint64_t sum;
	while ( C != 0){
		sum = (uint64_t)t[i]+(uint64_t)C;
		S = (uint32_t)sum;
		C = (uint32_t)(sum >> 32);
		t[i] = S;
		i++;
	}
}

void SUB_COND(uint32_t *res, uint32_t *n,uint32_t SIZE){
	if (!(larger_equal_than(n,res,SIZE,SIZE))){
		uint32_t temp[SIZE];
		uint32_t i;
		for (i=0;i<SIZE;i++){
			temp[i] = res[i];
		}
		mp_sub(temp, n, res, SIZE);
	}
}



