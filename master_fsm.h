/* master_fsm.h */

#ifndef MASTER_FSM_H_
#define MASTER_FSM_H_

#include <stdio.h>
#include <stdint.h>

extern int start_session;
extern int transmit_buffer;
extern int master_received;
extern int master_sent;
extern int stop;

void run_master();
void mac_encrypt(uint32_t *result, uint32_t* buffer, uint32_t payload_size, uint32_t *IV, uint32_t counter, uint32_t *key);
void send_msg_to_slave(uint32_t msg_type, uint32_t *message, uint32_t length);

#endif /* MASTER_FSM_H_ */
