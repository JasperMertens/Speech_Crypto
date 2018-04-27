/*montgomery2.h*/

#ifndef MONTGOMERY2_H_
#define MONTGOMERY2_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void mont_mult(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *n0, uint32_t *res, uint32_t SIZE);

#endif /* MONTGOMERY2_H_ */

