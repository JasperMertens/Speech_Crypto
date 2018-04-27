/*
 * format.h
 *
 */

#ifndef FORMAT_H_
#define FORMAT_H_

#include <stdint.h>
void arr32_to_arr8(uint8_t *res, uint32_t *input, int length);
void arr8_to_arr32(uint32_t *res, uint8_t *input, int length);
void arr32_to_arr16(short *res, uint32_t *input, int length);
void arr16_to_arr32(uint32_t *res, short *input, int length);
void rsa_encoded_message(uint8_t *res, uint8_t *hash_string, int em_length);

#endif /* FORMAT_H_ */
