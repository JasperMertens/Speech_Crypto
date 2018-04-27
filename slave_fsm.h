/* slave_fsm.h */

#ifndef SLAVE_FSM_H_
#define SLAVE_FSM_H_

#include <stdio.h>
#include <stdint.h>

extern int written_to_output;
extern int slave_received;
extern int slave_sent;

void run_slave();
int mac_decrypt(uint32_t *decrypt_result, uint32_t *input, uint32_t enc_ctr, uint32_t length);
void send_msg_to_master(uint32_t msg_type, uint32_t *message, uint32_t length);


#endif /* SLAVE_FSM_H_ */

