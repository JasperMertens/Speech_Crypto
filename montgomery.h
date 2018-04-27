/*montgomery.h*/

#ifndef MONTGOMERY_H_
#define MONTGOMERY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void mp_sub(uint32_t *a, uint32_t *b, uint32_t *res, uint32_t size);
void ADD(uint32_t *t, uint32_t i, uint32_t C);
void SUB_COND(uint32_t *res, uint32_t *n,uint32_t SIZE);
bool larger_equal_than(uint32_t *a, uint32_t *b,uint32_t size_a,uint32_t size_b);
void mont(uint32_t *a, uint32_t *b, uint32_t *n, uint32_t *n0, uint32_t *res, uint32_t SIZE);

#endif /* MONTGOMERY_H_ */
