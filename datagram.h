/* Datagram.h */

#ifndef DATAGRAM_H_
#define DATAGRAM_H_

#include <stdio.h>
#include <stdint.h>

void master_datagram(uint32_t *output, uint32_t *buffer, int N_blocks,uint32_t *iv, uint32_t counter, uint32_t *sym_key);
int slave_datagram(uint32_t *output, uint32_t *decrypt_buffer, int N_blocks,uint32_t *iv, uint32_t counter, uint32_t *sym_key);


#endif