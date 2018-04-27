/* padding.h */

#ifndef PADDING_H_
#define PADDING_H_


#include <stdint.h>

void addPadding(uint32_t *result, uint32_t *input, uint32_t input_byte_size, uint32_t output_byte_size);
/* Both result and output_size are outputs of removePadding */
void removePadding(uint32_t *result, uint32_t output_size, uint32_t *input, uint32_t input_size);


#endif /* PADDING_H_ */
