/*
 * handshake.h
 *
 */

#ifndef HANDSHAKE_H_
#define HANDSHAKE_H_

#include <stdint.h>
void master_handshake_1(uint32_t *g_x, uint32_t *x, uint32_t exp_length);
void slave_handshake_1(uint32_t *g_y, uint32_t *slave_signature, uint32_t *key, uint32_t *g_x, uint32_t *IV, uint32_t *y, uint32_t exp_length);
int master_handshake_2(uint32_t *master_signature, uint32_t *key, uint32_t *IV,
                            uint32_t *g_y, uint32_t *slave_signature, uint32_t *x, uint32_t exp_length, uint32_t *g_x);
int slave_handshake_2(uint32_t *master_signature, uint32_t *key, uint32_t *g_x, uint32_t *g_y, uint32_t *IV);


void sha256(uint32_t *output, uint32_t *input, unsigned int input_size);


#endif /* HANDSHAKE_H_ */

