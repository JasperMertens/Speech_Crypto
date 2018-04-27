/* master_fsm.c */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "master_fsm.h"
#include "slave_fsm.h"
#include "global.h"
#include "handshake.h"
#include "datagram.h"
#include "test.h"

/* Define the states */
#undef D_WAIT
#define INIT 0
#define H_SEND_1 1
#define H_WAIT 2
#define H_CHECK_2 3
#define H_SEND_3 4
#define D_WAIT 5
#define D_ENC 6

/* Define the substates */
#define NONE 0
#define WAIT_FOR_ACK 1
#define ACK_RECEIVED 2
#define TIMED_OUT 3
#define WAIT_FOR_REPLY 4
#define REPLY_RECEIVED 5
#define SEND_ACK 6
#define ACKNOWLEDGED 7

/* Define constants */
#define BUF_SIZE 4
#define MAX_CTR 1000
#define MAX_TIME 1000

/* Interfacing control signals */
int start_session = 0;
int transmit_buffer = 0;
int master_received = 0;
int master_sent = 0;
int stop = 0;

/* Local control signals */
int state;
int next_state;
int sub_state;
int next_sub_state;
int done;
int sub_done;
int timed_out;

/* Counters */
uint32_t seq_ctr;
uint32_t enc_ctr;

/* Local variables */
uint32_t x[5];
uint32_t g_x[32];
uint32_t key[4];
uint32_t IV[3];
uint32_t master_signature[40];
uint32_t start_time;

void run_master() {

    state = INIT;
    next_state = INIT;
    /*state = D_WAIT;
    next_state = D_WAIT;*/ /* for testing */
    sub_state = NONE;
    next_sub_state = NONE;

    /* for testing */
    while(stop == 0) {

        /* Run the sub-states code */
        if (sub_state == WAIT_FOR_ACK) {
            /*printf("Master sub_state: WAIT_FOR_ACK\r\n");*/
            master_sent = 1;
            sub_done = 0;

            if (clock() - start_time > MAX_TIME) {
                next_sub_state = TIMED_OUT;
            } else if (slave_received == 1) {
                next_sub_state = ACK_RECEIVED;
            }
        }

        else if (sub_state == ACK_RECEIVED) {
            /*printf("Master sub_state: ACK_RECEIVED\r\n");*/
            master_sent = 0;
            sub_done = 1;
            timed_out = 0;
            next_sub_state = NONE;
        }

        else if (sub_state == TIMED_OUT) {
            /*printf("Master sub_state: TIMED_OUT\r\n");*/
            master_sent = 0;
            master_received = 0;
            sub_done = 1;
            timed_out = 1;
            next_sub_state = NONE;
        }

        else if (sub_state == WAIT_FOR_REPLY) {
            /*printf("Master sub_state: WAIT_FOR_REPLY\r\n");*/
            sub_done = 0;
            if (clock() - start_time > MAX_TIME) {
                next_sub_state = TIMED_OUT;
            } else if (slave_sent == 1) {
                next_sub_state = REPLY_RECEIVED;
            }
        }

        else if (sub_state == REPLY_RECEIVED) {
            /*printf("Master sub_state: REPLY_RECEIVED\r\n");*/
            sub_done = 1;
            timed_out = 0;
            next_sub_state = NONE;
        }

        else if (sub_state == SEND_ACK) {
            /*printf("Master sub_state: SEND_ACK\r\n");*/
            master_received = 1;
            sub_done = 0;
            if (clock() - start_time > MAX_TIME) {
                next_sub_state = TIMED_OUT;
            } else if (slave_sent == 0) {
                next_sub_state = ACKNOWLEDGED;
            }
        }

        else if (sub_state == ACKNOWLEDGED) {
            /*printf("Master sub_state: ACKNOWLEDGED\r\n");*/
            master_received = 0;
            sub_done = 1;
            timed_out = 0;
            next_sub_state = NONE;
        }

        /* Run the main states code */
        else if (state == INIT) {
            printf("Master: INIT\r\n");
            seq_ctr = 0;
            enc_ctr = 0;
            if (start_session == 1) {
                next_state = H_SEND_1;
                done = 0;
                start_session = 0;
            }
        }

        else if (state == H_SEND_1) {
           /* printf("Master: H_SEND_1\r\n");*/
            if (done == 1) {
                done = 0;
                next_state = H_WAIT;
            } else {
                /* Generate random x */
                int i;
                for(i=0; i<5; i++) {x[i] = xA[i];}
                master_handshake_1(g_x, x, 160);
                send_msg_to_slave(0, g_x, 32);
            }
        }

        else if (state == H_WAIT) {
            /*printf("Master: H_WAIT\r\n");*/
            if (done == 1) {
                done = 0;
                next_state = H_CHECK_2;
            } else {
                if (sub_done == 1) {
                    sub_done = 0;
                    if (timed_out == 1) {
                        fprintf(stderr, "Master FSM: Waiting for handshake reply timed out!\r\n");
                        next_state = INIT;
                        abort();
                    } else {
                        done = 1;
                    }
                } else {
                    start_time = clock();
                    next_sub_state = WAIT_FOR_REPLY;
                }
            }
        }

        else if (state == H_CHECK_2) {
            /*printf("Master: H_CHECK_2\r\n");*/
            if (done == 1) {
                done = 0;
                next_state = H_SEND_3;
            } else {
                if (sub_done == 1) {
                    sub_done = 0;
                    if (timed_out == 1) {
                        fprintf(stderr, "Master FSM: Waiting for acknowledgement timed out!\r\n");
                        next_state = INIT;
                        abort();
                    } else {
                        done = 1;
                        seq_ctr++;
                        if (seq_ctr >= MAX_CTR) {
                            next_state = INIT;
                            start_session = 1;
                        }
                    }
                } else {
                    /* Check header */
                    if (in_transmission[0]==0 && in_transmission[1]==1) {
                        uint32_t g_y[32];
                        uint32_t slave_signature[40];
                        int alpha_verify;
                        int i;
                        for (i=0; i<3; i++) {IV[i] = in_transmission[i+2];}
                        for (i=0; i<32; i++) {g_y[i] = in_transmission[i+5];}
                        for (i=0; i<40; i++) {slave_signature[i] = in_transmission[i+37];}
                        alpha_verify = master_handshake_2(master_signature, key, IV,
                                g_y, slave_signature, x, 160, g_x);
                        if (alpha_verify == 1) {
                                start_time = clock();
                                next_sub_state = SEND_ACK;
                        } else {
                            fprintf(stderr, "Master FSM: Invalid handshake message 2 received!\r\n");
                            next_sub_state = WAIT_FOR_REPLY;
                            abort();
                        }
                    } else {
                        fprintf(stderr, "Master FSM: Invalid header received!\r\n");
                        next_state = WAIT_FOR_REPLY;
                        abort();
                    }
                }
            }
        }

        else if (state == H_SEND_3) {
            /*printf("Master: H_SEND_3\r\n");*/
            if (done == 1) {
                done = 0;
                next_state = D_WAIT;
            } else {
                send_msg_to_slave(0, master_signature, 40);
            }
        }

        else if (state == D_WAIT) {
            /*printf("Master: D_WAIT\r\n");*/
            if (transmit_buffer == 1) {
                next_state = D_ENC;
            }
        }

        else if (state == D_ENC) {
            /*printf("Master: D_ENC\r\n");*/
            if (done == 1) {
                done = 0;
                next_state = D_WAIT;
            } else {
                uint32_t result[(BUF_SIZE+2)*4+1];
                mac_encrypt(result, input_buffer,BUF_SIZE,IV,enc_ctr,key);
                send_msg_to_slave(1, result, BUF_SIZE+9);
            }
        }

        else {
            fprintf(stderr, "Master FSM: Illegal State!\r\n");
            printf("State: %d, Substate: %d\r\n", state, sub_state);
            abort();
        }

        if (next_state != state) {
            printf("Master FSM next state: %d\r\n", next_state);
        }

        state = next_state;
        sub_state = next_sub_state;

        run_slave();
    }
    printf("finished\r\n");
}

void mac_encrypt(uint32_t *result, uint32_t* buffer, uint32_t payload_size, uint32_t *IV, uint32_t counter, uint32_t *key) {
    uint32_t datagram_output[BUF_SIZE+8];
    int i;
    master_datagram(datagram_output,buffer,payload_size/4,IV,counter,key);
    result[0] = payload_size;
    for (i=0; i<payload_size+8;i++){
        result[1+i] = datagram_output[i];
    }
}


void send_msg_to_slave(uint32_t msg_type, uint32_t *message, uint32_t length) {
    printf("Sending message to slave\r\n");
    if (sub_done == 1) {
        sub_done = 0;
        if (timed_out == 1) {
            fprintf(stderr, "Master FSM: Sending message timed out!\r\n");
            next_state = INIT;
            abort();
        } else {
            seq_ctr++;
            if (seq_ctr >= MAX_CTR) {
                next_state = INIT;
                start_session = 1;
            }
            done = 1;
        }
    } else {
        int i;
        in_transmission[0] = msg_type;
        in_transmission[1] = seq_ctr;
        for (i=0; i<length; i++) {
            in_transmission[i+2] = message[i];
        }
        start_time = clock();
        next_sub_state = WAIT_FOR_ACK;
    }
}





