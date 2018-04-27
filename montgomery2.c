/*montgomery2.c*/

#include <stdio.h>
#include <stdint.h>

#include "montgomery2.h"

void add_mont(uint32_t *t, uint32_t i, uint32_t C);
void sub_cond_mont(uint32_t *t, uint32_t *n, uint32_t *p, uint32_t SIZE);

#define MAX_SIZE 39


void mont_mult(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *n0, uint32_t *res, uint32_t SIZE) {
	uint32_t t[MAX_SIZE+2] = {0};
	uint32_t i;
	uint32_t j=0;
	uint64_t sum=0;
	uint32_t C=0;
	uint32_t S=0;
	uint32_t m=0;
	for(i=0 ; i<SIZE ; i++) {
		sum = (uint64_t)t[0] + (uint64_t)a[0]*(uint64_t)b[i];
		C = (sum>>32);
		S = (uint32_t)(sum);
		add_mont(t,1,C);
		m = (uint32_t)(S*(*n0));
		sum = (uint64_t)S + (uint64_t)m*(uint64_t)n[0];
		C = (sum>>32);
		S = (uint32_t)(sum);
		for(j=1; j<SIZE ; j++) {
			sum = (uint64_t)t[j] + (uint64_t)a[j]*(uint64_t)b[i] + C;
			C = (sum>>32);
			S = (uint32_t)(sum);
			add_mont(t,j+1,C);
			sum = (uint64_t)S + (uint64_t)m*(uint64_t)n[j];
			C = (sum>>32);
			S = (uint32_t)(sum);
			t[j-1] = S;
		}
		sum = (uint64_t)t[SIZE] + (uint64_t)C;
		C = (sum>>32);
		S = (uint32_t)(sum);
		t[SIZE-1] = S;
		t[SIZE] = t[SIZE+1] + C;
		t[SIZE+1] = 0;
	}
	sub_cond_mont(t,n, res, SIZE);
}

void add_mont(uint32_t *t, uint32_t i, uint32_t C){
	uint64_t sum = 0;
	uint32_t S;
	while(C!=0) {
		sum = (uint64_t)t[i] + (uint64_t)C;
		C = (sum>>32);
		S = (uint32_t)(sum);
		t[i] = S;
		i++;
	}
}


void sub_cond_mont(uint32_t *u, uint32_t *n, uint32_t *p, uint32_t SIZE){
	uint32_t B = 0;
	uint32_t t[MAX_SIZE];
	uint32_t sub = 0;
	uint32_t i;
	for(i=0 ; i<SIZE ; i++) {
		sub = u[i] - n[i] - B;
		if(u[i] >= n[i] + B){
			B = 0;
		} else {
			B = 1;
		}
		t[i] = sub;
	}
	if (u[SIZE] == 1) {
        B = 0;
	}
	if(B == 0){
        for (i=0; i<SIZE; i++) {
			p[i] = t[i];
		}
	} else {
		for (i=0; i<SIZE; i++) {
			p[i] = u[i];
		}
	}
}
