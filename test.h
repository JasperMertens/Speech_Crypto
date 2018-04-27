/*
 * test.h
 *
 */

#ifndef TEST_H_
#define TEST_H_

#include <stdint.h>
int test_format();
int test_aes();
int test_ctr_mode();
int test_sha2();
int test_montgomery();
int test_exponentiation();
int test_padding();
int test_handshake();
int test_datagram();
int test_fsm_data_transmission();
int test_rsa_encoded_message();
int test_rsa();
int test_data_exchange();
void printArray(uint32_t *array, int size);
void printCharArray(unsigned char *array, int size);

#endif /* TEST_H_ */
