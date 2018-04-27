/* slave_fsm.c */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "master_fsm.h"
#include "slave_fsm.h"
#include "global.h"
#include "handshake.h"
#include "test.h"
#include "datagram.h"

/* Define the states */
#undef D_WAIT
#define INIT 0
#define H_SEND_2 1
#define H_WAIT 2
#define H_CHECK_3 3
#define D_WAIT 4
#define D_DEC 5
#define D_WRITE 6

/* Define the sub-states */
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
int written_to_output = 0;
int slave_received = 0;
int slave_sent = 0;

/* Local control signals */
int slave_state = INIT;
int next_slave_state = INIT;
/*
int slave_state = D_WAIT;
int next_slave_state = D_WAIT;*/ /* for testing */
int slave_sub_state = NONE;
int next_slave_sub_state = NONE;
int slave_done;
int slave_sub_done;
int slave_timed_out;

/* Counters */
uint32_t slave_seq_ctr = 0;
uint32_t slave_enc_ctr;

/* Local variables */
uint32_t y[5];
uint32_t g_x[32];
uint32_t g_y[32];
uint32_t key[4];
uint32_t IV[3];
uint32_t slave_signature[40];
uint32_t decrypt_result[BUF_SIZE];
uint32_t slave_start_time;

void run_slave() {

        /* Run the sub-states code */
        if (slave_sub_state == WAIT_FOR_ACK) {
            /*printf("Slave sub_state: WAIT_FOR_ACK\r\n");*/
            slave_sent = 1;
            slave_sub_done = 0;

            if (clock() - slave_start_time > MAX_TIME) {
                next_slave_sub_state = TIMED_OUT;
            } else if (master_received == 1) {
                next_slave_sub_state = ACK_RECEIVED;
            }
        }

        else if (slave_sub_state == ACK_RECEIVED) {
            /*printf("Slave sub_state: ACK_RECEIVED\r\n");*/
            slave_sent = 0;
            slave_sub_done = 1;
            slave_timed_out = 0;
            next_slave_sub_state = NONE;
        }

        else if (slave_sub_state == TIMED_OUT) {
            /*printf("Slave sub_state: TIMED_OUT\r\n");*/
            slave_sent = 0;
            slave_received = 0;
            slave_sub_done = 1;
            slave_timed_out = 1;
            next_slave_sub_state = NONE;
        }

        else if (slave_sub_state == WAIT_FOR_REPLY) {
            /*printf("Slave sub_state: WAIT_FOR_REPLY\r\n");*/
            slave_sub_done = 0;
            if (clock() - slave_start_time > MAX_TIME) {
                next_slave_sub_state = TIMED_OUT;
            } else if (master_sent == 1) {
                next_slave_sub_state = REPLY_RECEIVED;
            }
        }

        else if (slave_sub_state == REPLY_RECEIVED) {
            /*printf("Slave sub_state: REPLY_RECEIVED\r\n");*/
            slave_sub_done = 1;
            slave_timed_out = 0;
            next_slave_sub_state = NONE;
        }

        else if (slave_sub_state == SEND_ACK) {
            /*printf("Slave sub_state: SEND_ACK\r\n");*/
            slave_received = 1;
            slave_sub_done = 0;
            if (clock() - slave_start_time > MAX_TIME) {
                next_slave_sub_state = TIMED_OUT;
            } else if (master_sent == 0) {
                next_slave_sub_state = ACKNOWLEDGED;
            }
        }

        else if (slave_sub_state == ACKNOWLEDGED) {
            /*printf("Slave sub_state: ACKNOWLEDGED\r\n");*/
            slave_received = 0;
            slave_sub_done = 1;
            slave_timed_out = 0;
            next_slave_sub_state = NONE;
        }

    /* Run the main states code */
        else if (slave_state == INIT) {
            /*printf("Slave: INIT\r\n");*/
            slave_seq_ctr = 0;
            slave_done = 0;
            if (slave_sub_done == 1) {
                slave_sub_done = 0;
                if (slave_timed_out == 1) {
                    fprintf(stderr, "Slave FSM: Waiting for acknowledgement timed out!\r\n");
                    next_slave_state = INIT;
                    abort();
                } else {
                    slave_seq_ctr++;
                    next_slave_state = H_SEND_2;
                }
            } else if (master_sent == 1) {
                /* Check header, if valid copy g_x, put slave_received high
                    until master_sent is low */
                if (in_transmission[0] == 0 && in_transmission[1] == 0) {
                    int i;
                    for (i=0; i<32; i++) {g_x[i] = in_transmission[i+2];}
                    slave_start_time = clock();
                    next_slave_sub_state = SEND_ACK;
                }
            }
        }

        else if (slave_state == H_SEND_2) {
            /*printf("Slave: H_SEND_2\r\n");*/
            if (slave_done == 1) {
                slave_done = 0;
                next_slave_state = H_WAIT;
            } else {
                uint32_t concat[75];
                /* Generate random y */
                int i;
                for(i=0; i<5; i++) {y[i] = xB[i];}
                /* Generate a 'random' IV */
                IV[0] = 0x12345678; IV[1] = 0x12345678; IV[2] = 0x12345678;
                slave_handshake_1(g_y, slave_signature, key, g_x, IV, y, 160);
                /* Concatenate IV[3], g_y[32] and slave_signature[40] */
                for (i=0; i<3; i++) { concat[i] = IV[i];}
                for (i=0; i<32; i++) { concat[i+3] = g_y[i];}
                for (i=0; i<40; i++) { concat[i+35] = slave_signature[i];}
                send_msg_to_master(0, concat, 75);
            }
        }

        else if (slave_state == H_WAIT) {
            /*printf("Slave: H_WAIT\r\n");*/
            if (slave_done == 1) {
                slave_done = 0;
                next_slave_state = H_CHECK_3;
            } else {
                if (slave_sub_done == 1) {
                    slave_sub_done = 0;
                    if (slave_timed_out == 1) {
                        fprintf(stderr, "Slave FSM: Waiting for handshake reply timed out!\r\n");
                        next_slave_state = INIT;
                        abort();
                    } else {
                        slave_done = 1;
                    }
                } else {
                    slave_start_time = clock();
                    next_slave_sub_state = WAIT_FOR_REPLY;
                }
            }
        }

        else if(slave_state == H_CHECK_3) {
            /*printf("Slave: H_CHECK_3\r\n");*/
            if (slave_done == 1) {
                slave_done = 0;
                next_slave_state = D_WAIT;
            } else {
                if (slave_sub_done == 1) {
                    slave_sub_done = 0;
                    if (slave_timed_out == 1) {
                        fprintf(stderr, "Slave FSM: Waiting for acknowledgement timed out!\r\n");
                        next_slave_state = INIT;
                        abort();
                    } else {
                        slave_seq_ctr++;
                        if (slave_seq_ctr >= MAX_CTR) {
                            next_slave_state = INIT;
                        }
                        slave_done = 1;
                    }
                } else {
                    /* Check header */
                    if (in_transmission[0]==0 && in_transmission[1]==2) {
                        uint32_t master_signature[40];
                        int i;
                        int beta_verify;

                        for (i=0; i<40; i++) { master_signature[i] = in_transmission[i+2];}
                        beta_verify = slave_handshake_2(master_signature, key, g_x, g_y, IV);
                        if (beta_verify == 1) {
                            slave_start_time = clock();
                            next_slave_sub_state = SEND_ACK;
                        } else {
                            fprintf(stderr, "Slave FSM: Invalid handshake message 3 received!\r\n");
                            next_slave_sub_state = WAIT_FOR_REPLY;
                            abort();
                        }
                    } else {
                        fprintf(stderr, "Slave FSM: Invalid header received!\r\n");
                        next_slave_state = WAIT_FOR_REPLY;
                        abort();
                    }
                }
            }
        }


        else if (slave_state == D_WAIT) {
            /*printf("Slave: D_WAIT\r\n");*/
            if (master_sent == 1) {
                 /* Check header */
                if (in_transmission[0]==1 && in_transmission[1]==slave_seq_ctr) {
                    next_slave_state = D_DEC;
                } else {
                    next_slave_state = INIT;
                    fprintf(stderr, "Slave FSM: Invalid header!\r\n");
                    abort();
                }
            }
        }

        else if (slave_state == D_DEC) {
            /*printf("Slave: D_DEC\r\n");*/
            if (slave_done == 1) {
                next_slave_state = D_WRITE;
                slave_done = 0;
            } else {
                if (slave_sub_done == 1) {
                    slave_sub_done = 0;
                    if (slave_timed_out == 1) {
                        fprintf(stderr, "Slave FSM: Waiting for acknowledgement timed out!\r\n");
                        next_slave_state = INIT;
                        abort();
                    } else {
                        slave_done = 1;
                        slave_seq_ctr++;
                        if (slave_seq_ctr >= MAX_CTR) {
                            next_slave_state = INIT;
                        }
                    }
                } else {
                    int check = mac_decrypt(decrypt_result, in_transmission+2, slave_enc_ctr, BUF_SIZE);
                    if (check == 1) {
                        fprintf(stderr, "Slave FSM: MAC invalid!\r\n");
                        next_slave_state = INIT;
                        abort();
                    } else {
                        slave_start_time = clock();
                        next_slave_sub_state = SEND_ACK;
                    }
                }
            }
        }

        else if (slave_state == D_WRITE) {
            /*printf("Slave: D_WRITE\r\n");*/
            if (slave_done == 1) {
                next_slave_state = D_WAIT;
            } else {
                /* Write decryption result to output buffer and signal when it's ready*/
                int i;
                for (i=0; i<BUF_SIZE; i++) {
                    output_buffer[i] = decrypt_result[i];
                }
                written_to_output = 1;
                slave_done = 1;
                stop = 1;
            }
        }

        else {
            fprintf(stderr, "Slave FSM: Illegal State!\r\n");
            abort();
        }

        if (next_slave_state != slave_state) {
            printf("Slave FSM next state: %d\r\n", next_slave_state);
        }

        slave_state = next_slave_state;
        slave_sub_state = next_slave_sub_state;
}

int mac_decrypt(uint32_t *decrypt_result, uint32_t *input, uint32_t slave_enc_ctr, uint32_t length) {
    uint32_t N_blocks = input[0]/4+2;
    input++;
    return slave_datagram(decrypt_result,input,N_blocks,IV,slave_enc_ctr,key);
}

void send_msg_to_master(uint32_t msg_type, uint32_t *message, uint32_t length) {
    printf("Sending message to master\r\n");
    if (slave_sub_done == 1) {
        slave_sub_done = 0;
        if (slave_timed_out == 1) {
            fprintf(stderr, "Slave FSM: Sending message timed out!\r\n");
            next_slave_state = INIT;
            abort();
        } else {
            slave_seq_ctr++;
            if (slave_seq_ctr >= MAX_CTR) {
                next_slave_state = INIT;
            }
            slave_done = 1;
        }
    } else {
        int i;
        in_transmission[0] = msg_type;
        in_transmission[1] = slave_seq_ctr;
        for (i=0; i<length; i++) {
            in_transmission[i+2] = message[i];
        }
        slave_start_time = clock();
        next_slave_sub_state = WAIT_FOR_ACK;
    }
}
