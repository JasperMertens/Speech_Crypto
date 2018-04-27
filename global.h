/* global.h */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdint.h>
/* 80-bit security if p has length 1248*/

/* prime modulus */
extern uint32_t p[32];
/* generator */
extern uint32_t g[32];
/* 2^(1024) mod p */
extern uint32_t R_1024[32];
/* R_1024^2 mod p */
extern uint32_t R2_1024[32];
/* -p^(-1) mod 2^(1024) */
extern uint32_t p_prime[32];
/* secret key of A */
extern uint32_t xA[5];
/* public key of A */
extern uint32_t yA[32];
/* secret key of B */
extern uint32_t xB[5];
/* public key of B */
extern uint32_t yB[32];
/* shared result of DH */
extern uint32_t Z[32] ;

/* RSA parameters */

/* RSA modulus */
extern uint32_t rsa_modulus[39];
/* -r^(-1) mod 2^(1024) */
extern uint32_t r_prime[39];
/* R mod rsa_modulus */
uint32_t R_rsa[39];
/* R^2 mod rsa_modulus */
uint32_t R2_rsa[39];
/* private decryption exponent of slave */
extern uint32_t ds[39];
/* public encryption exponent of slave */
extern uint32_t es[1];


/* Shared memory */
extern uint32_t input_buffer[4];
extern uint32_t in_transmission[128];
extern uint32_t output_buffer[4];

#define BUFFERSIZE 40
#define BUFFERSIZE2 20
#define BUFFERSIZE4 10
#define BUFFERSIZE8 5

#define filterLength1 10
#define filterLength2 16

#define nbDeltas 20

#define mu1 8809
#define mu2 -2034
#define mu3 1
#define mu4 -1

#define INPUTWAVFILE  "input.wav"
#define OUTPUTWAVFILE "output.wav"

#define VERBOSE



#endif
