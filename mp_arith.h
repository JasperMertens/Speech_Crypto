/*
 * mp_arith.h
 *
 */

#ifndef MP_ARITH_H_
#define MP_ARITH_H_

#include <stdint.h>
void mp_sub(uint32_t *a, uint32_t *b, uint32_t *res);
void mp_add(uint32_t *a, uint32_t *b, uint32_t *res);

#endif /* MP_ARITH_H_ */
