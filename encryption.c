/*encryption.c*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "aes.h"
#include "format.h"
#include "montgomery.h"
#include "montgomery2.h"
#include "test.h"
#include "global.h"


void ctr_mode(uint32_t *output, uint32_t *padded_input, int N_blocks, uint32_t *iv, uint32_t counter, uint8_t *sym_key) {
	aes_key key;
	unsigned char cipher_strm[16];
	uint32_t converted_strm[4];

	uint8_t nonce[16];

	int i,j;
	uint32_t *t_out = output;
	uint32_t *t_in = padded_input;
	uint32_t t_ctr = counter;

	arr32_to_arr8(nonce, iv, 3);
	aes_set_encrypt_key(&key, sym_key, 128);

	for (i=0; i<N_blocks; i++) {
		nonce[12] = (uint8_t)(t_ctr >> 24);
		nonce[13] = (uint8_t)(t_ctr >> 16);
		nonce[14] = (uint8_t)(t_ctr >>  8);
		nonce[15] = (uint8_t) t_ctr;
		t_ctr++;

		aes_encrypt(&key, nonce, cipher_strm);
		arr8_to_arr32(converted_strm, cipher_strm, 16);
		for (j=0; j<4; j++) {
			t_out[j] = converted_strm[j] ^ t_in[j];
		}
		t_out += 4;
		t_in += 4;
	}
}

/*Calculates res = (x^exp) mod N*/
void mod_exp(uint32_t *base, uint32_t *exp, uint32_t exp_len, uint32_t *n, uint32_t *n_prime, uint32_t *R_1024, uint32_t *R2_1024, uint32_t size, uint32_t *res) {
	uint32_t One[39] = {1,0};
	int i;
	int bit;

	uint32_t x_tilde[39];
	uint32_t *A = res;

	/*Calculate x_tilde = MontMul(x, R^2 mod m)*/
	mont_mult(base, R2_1024, n, n_prime, x_tilde, size);

	/* Copy R to A*/
	for(i = 0; i < size; i++) {
	    A[i] = R_1024[i];
	}
	while(exp_len>0)
	{
		exp_len--;
		bit = (exp[exp_len/32] >> (exp_len%32)) & 1;
		/* Calculate A = MontMul(A, A)*/
		mont_mult(A, A, n, n_prime, A, size);
		if(bit)
		{
			/* Calculate A = MontMul(A, x_tilde)*/
			mont_mult(A, x_tilde, n, n_prime, A, size);
		}
	}
	/* Calculate A = MontMul(A, 1)*/
	mont_mult(A, One, n, n_prime, A, size);
}

void rsa_sign(uint32_t *result, uint32_t *hash_input) {
    uint8_t hash_string[32];
    uint8_t char_enc_msg[156];
    uint32_t enc_msg[39];
    arr32_to_arr8(hash_string, hash_input, 8);
    rsa_encoded_message(char_enc_msg, hash_string, 156);
    arr8_to_arr32(enc_msg, char_enc_msg, 156);
    mod_exp(enc_msg, ds, 1248, rsa_modulus, r_prime, R_rsa, R2_rsa, 39, result);
}

int rsa_verify(uint32_t *signed_input, uint32_t *validation_input) {
    uint32_t to_validate[39];
    uint8_t val_string[32];
    uint8_t char_enc_msg[156];
    uint32_t enc_msg[39];
    int verif_res = 0;

    mod_exp(signed_input, es, 20, rsa_modulus, r_prime, R_rsa, R2_rsa, 39, to_validate);
    arr32_to_arr8(val_string, validation_input, 8);
    rsa_encoded_message(char_enc_msg, val_string, 156);
    arr8_to_arr32(enc_msg, char_enc_msg, 156);
    if (memcmp(to_validate, enc_msg, 39) == 0) {
        verif_res = 1;
    }

    return verif_res;
}
