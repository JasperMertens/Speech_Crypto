/* format.c */

#include <stdio.h>
#include <stdint.h>
#include "digest-info.h"

void arr32_to_arr8(uint8_t *res, uint32_t *input, int length) {
	int i;
	uint8_t *r_pt = res;
	uint32_t input_word;
	for(i=0; i<length; i++) {
		input_word = input[i];
		r_pt[0] = (uint8_t)(input_word >> 24);
		r_pt[1] = (uint8_t)(input_word >> 16);
		r_pt[2] = (uint8_t)(input_word >>  8);
		r_pt[3] = (uint8_t) input_word;
		r_pt += 4;
	}
}

void arr8_to_arr32(uint32_t *res, uint8_t *input, int length) {
	int i;
	uint8_t *i_pt = input;
	for (i=0; i<length/4; i++) {
		res[i] = ((uint32_t)(i_pt)[0] << 24) ^ ((uint32_t)(i_pt)[1] << 16) ^ ((uint32_t)(i_pt)[2] <<  8) ^ ((uint32_t)(i_pt)[3]);
		i_pt += 4;
	}
}

void arr32_to_arr16(short *res, uint32_t *input, int length) {
	int i;
	short *r_pt = res;
	uint32_t input_word;
	
	for(i=0; i<length; i++) {
		input_word = input[i];
		r_pt[0] = (short)(input_word >> 16);
		r_pt[1] = (short)(input_word);
		r_pt += 2;
	}
}

void arr16_to_arr32(uint32_t *res, short *input, int length) {
	int i;
	short *i_pt = input;
	uint16_t temp1;
	uint16_t temp2;
	for (i=0; i<length/2; i++) {
	    temp1 = (uint16_t)(i_pt)[0];
	    temp2 = (uint16_t)(i_pt)[1];
	    res[i] = ((uint32_t)temp1 << 16) ^ ((uint32_t)temp2) ;
	    i_pt += 2;
	}
}

/* Make sure em_length >= 62 octets */
void rsa_encoded_message(uint8_t *res, uint8_t *hash_string, int em_length) {
    int i;
    res[0] = 0;
    res[1] = 1;
    for (i=0; i<em_length-54; i++){ res[i+2] = 0xff;}
    res[em_length-52] = 0;
    for (i=0; i<19; i++) { res[i+em_length-51] = sha256_prefix[i];}
    for (i=0; i<32; i++) { res[i+em_length-32] = hash_string[i];}
}
