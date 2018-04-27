/* handshake.c */

#include <stdio.h>
#include <stdint.h>
#include "global.h"
#include "encryption.h"
#include "format.h"
/*#include "sha2.h"*/
#include "sha256.h"
#include "padding.h"
#include "test.h"

/* unsigned char digest_string[SIZE*2+1]; */
void sha256(uint32_t *output, uint32_t *input, unsigned int input_size){

    BYTE buf[SHA256_BLOCK_SIZE] = {0};
	SHA256_CTX ctx;
    unsigned char hash_input[268] = {0};

    arr32_to_arr8(hash_input, input, input_size);
    sha256_init(&ctx);
	sha256_update(&ctx, hash_input, input_size*4);
	sha256_final(&ctx, buf);
	arr8_to_arr32(output, buf, 32);
}

/* Returns g_x[32] */
void master_handshake_1(uint32_t *g_x, uint32_t *x, uint32_t exp_length) {
    mod_exp(g, x, exp_length, p, p_prime, R_1024, R2_1024, 32, g_x);
}


/* Returns g_y[32] and slave_signature[40] */
void slave_handshake_1(uint32_t *g_y, uint32_t *slave_signature, uint32_t *key, uint32_t *g_x, uint32_t *IV, uint32_t *y, uint32_t exp_length) {
    uint32_t g_xy[32];
    uint32_t temp_key[8];

    uint32_t concat[67];
    int i;

    uint32_t alpha[39] = {0};
    uint32_t signed_alpha[39];

    uint32_t padded_input[40];
    uint32_t input_byte_size = 156;
    uint32_t output_byte_size = 160;

    uint32_t counter = 0;
    uint8_t key_char[16];

    /* Compute g^y */
    mod_exp(g, y, exp_length, p, p_prime, R_1024, R2_1024, 32, g_y);

    /* Compute (g^x)^y */
    mod_exp(g_x, y, exp_length, p, p_prime, R_1024, R2_1024, 32, g_xy);

    /* The key is the first 128 bits of the hash of (g^x)^y */
    sha256(temp_key, g_xy, 32);
    for (i=0; i<4; i++) { key[i]= temp_key[i];}

    /* Concatenate IV[3], g_y[32] and g_x[32] */
    for (i=0; i<3; i++) { concat[i] = IV[i];}
    for (i=0; i<32; i++) { concat[i+3] = g_y[i];}
    for (i=0; i<32; i++) { concat[i+35] = g_x[i];}

    /* Hash the concatenation to produce alpha */
    sha256(alpha, concat, 67);

    /* Sign alpha with the slave's private exponent and encrypt with CTR mode */
    /*mod_exp(alpha, ds, 1248, rsa_modulus, r_prime, R_rsa, R2_rsa, 39, signed_alpha);*/
    rsa_sign(signed_alpha, alpha);
    addPadding(padded_input, signed_alpha, input_byte_size, output_byte_size);
    arr32_to_arr8(key_char, key, 4);
    ctr_mode(slave_signature, padded_input, 10, IV, counter, key_char);
}

/* Returns master_signature[40], a verification of the signature (verif_res) and key[4] */
int master_handshake_2(uint32_t *master_signature, uint32_t *key, uint32_t *IV,
                            uint32_t *g_y, uint32_t *slave_signature, uint32_t *x, uint32_t exp_length, uint32_t *g_x) {
    uint32_t g_xy[32];
    uint32_t temp_key[8];

    uint32_t concat[67];
    int i;
    uint32_t alpha_prime[8];

    uint32_t counter = 0;
    uint8_t key_char[16];
    uint32_t decrypted_sign[40];
    uint32_t concat2[64];

    uint32_t beta[8];

    uint32_t signed_beta[39];
    uint32_t padded_input[40];
    uint32_t input_byte_size = 156;
    uint32_t output_byte_size = 160;
    uint32_t counter2 = 40;
    int verif_res = 0;

    /* Compute (g^y)^x */
    mod_exp(g_y, x, exp_length, p, p_prime, R_1024, R2_1024, 32, g_xy);

    /* The key is the first 128 bits of the hash of (g^x)^y */
    sha256(temp_key, g_xy, 32);
    for (i=0; i<4; i++) { key[i] = temp_key[i];}

    /* Concatenate IV[3], g_y[32] and g_x[32] */
    for (i=0; i<3; i++) { concat[i] = IV[i];}
    for (i=0; i<32; i++) { concat[i+3] = g_y[i];}
    for (i=0; i<32; i++) { concat[i+35] = g_x[i];}

    /* Hash the concatenation to produce alpha_prime */
    sha256(alpha_prime, concat, 67);

    /* Decrypt the slave's signature */
    arr32_to_arr8(key_char, key, 4);
    ctr_mode(decrypted_sign, slave_signature, 10, IV, counter, key_char);
    /*mod_exp(decrypted_sign, es, 20, rsa_modulus, r_prime, R_rsa, R2_rsa, 39, alpha);*/
    verif_res = rsa_verify(decrypted_sign, alpha_prime);

    /* Concatenate g_x[32] and g_y[32] */
    for (i=0; i<32; i++) {
        concat2[i] = g_x[i];
        concat2[i+32] = g_y[i];
    }

    /* Hash the concatenation to produce beta */
    sha256(beta, concat2, 64);

    /* Sign beta with the master's private exponent and encrypt with CTR mode */
    /*mod_exp(beta, ds, 1248, rsa_modulus, r_prime,  R_rsa, R2_rsa, 39, signed_beta);*/
    rsa_sign(signed_beta, beta);
    addPadding(padded_input, signed_beta, input_byte_size, output_byte_size);
    ctr_mode(master_signature, padded_input, 10, IV, counter2, key_char);

    return verif_res;
}

/* Returns a verification of the signature (verif_res) */
int slave_handshake_2(uint32_t *master_signature, uint32_t *key, uint32_t *g_x, uint32_t *g_y, uint32_t *IV) {

    uint32_t concat[64];
    int i;
    uint32_t beta_prime[8];
    uint32_t counter = 40;
    uint8_t key_char[16];
    uint32_t decrypted_sign[40];

    /* Concatenate g_x[32] and g_y[32] */
    for (i=0; i<32; i++) {
        concat[i] = g_x[i];
        concat[i+32] = g_y[i];
    }

    /* Hash the concatenation to produce beta_prime */
    sha256(beta_prime, concat, 64);

    /* Decrypt the master's signature */
    arr32_to_arr8(key_char, key, 4);
    ctr_mode(decrypted_sign, master_signature, 10, IV, counter, key_char);
    /*mod_exp(decrypted_sign, es, 20, rsa_modulus, r_prime,  R_rsa, R2_rsa, 39, beta);*/
    return rsa_verify(decrypted_sign, beta_prime);
}
