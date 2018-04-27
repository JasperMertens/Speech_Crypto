/*encryption.h*/

#ifndef ENCRYPTION_H_
#define ENCRYPTION_H_

#include <stdio.h>
#include <stdint.h>


void ctr_mode(uint32_t *output, uint32_t *padded_input, int N_blocks, uint32_t *iv, uint32_t counter, uint8_t *sym_key);
void mod_exp(uint32_t *base, uint32_t *exp, uint32_t exp_len, uint32_t *n, uint32_t *n_prime, uint32_t *R_1024, uint32_t *R2_1024, uint32_t size, uint32_t *res);
void rsa_sign(uint32_t *result, uint32_t *hash_input);
int rsa_verify(uint32_t *signed_input, uint32_t *validation_input);
#endif /* ENCRYPTION_H_ */
