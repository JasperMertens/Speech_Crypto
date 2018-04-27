/* Datagram */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "encryption.h"
#include "format.h"
#include "test.h"

#define MAX_N_BLOCKS 8

/* Output = E_key(TAG_key, Payload_length (# of blocks of data being sent),Message+padding)
   Buffer = payload (data being sent from buffer)
   N_blocks = number of blocks in buffer
   Counter = starting counter (for first block)
   COUNTER INFORMATION: after master datagram, the next counter should be //2*N_blocks + 3// higher
   Sym_key = symmetric key 
*/
void master_datagram(uint32_t *output, uint32_t *buffer, int N_blocks,uint32_t *iv, uint32_t counter, uint32_t *sym_key){ /* Method to send data */
    uint32_t to_mac[(MAX_N_BLOCKS+1)*4] = {0};
    uint32_t from_mac[(MAX_N_BLOCKS+1)*4] = {0};
    uint32_t to_mac_size = N_blocks+1;

    uint8_t char_key[16] = {0};
    int i;
    uint32_t to_encrypt[(MAX_N_BLOCKS+1)*4+4] = {0};
    uint32_t to_encrypt_size = to_mac_size+1;

    /* convert sym_key to 8bit key */
    arr32_to_arr8(char_key,sym_key,4);

    /* determine TAGk = CBC MAC of payload length and M') */
    /* Pad the data in the buffer */
    to_mac[0] = N_blocks; /* first 32-bits are Payload length (= # of 128-bit blocks) of the message*/

    for (i=0; i<N_blocks*4; i++) {
        to_mac[i+1] = buffer[i]; /* next bits are actual message */
    }
    for (i=0; i<3; i++){
        to_mac[N_blocks*4+1+i] = 0x0c0c0c0c; /* 3 32-bit numbers or 12 bytes need to be padded
        789516 = 0x0c0c0c = 12 12 12 */
    }
    ctr_mode(from_mac,to_mac,to_mac_size,iv,counter,char_key);
    /* Apply authenticated encryption */
    for (i=0; i<to_mac_size*4; i++){
        to_encrypt[i+4] = to_mac[i]; /* copy but with 1 block offset for TAG */
    }
    for (i=0; i<4; i++){
        to_encrypt[i] = from_mac[to_mac_size*4-4+i]; /* TAGk = last block of from_mac */
    }
    /* Increase counter after using it for CBC-MAC */
    counter += to_mac_size;
    ctr_mode(output,to_encrypt,to_encrypt_size,iv,counter,char_key);
}

int slave_datagram(uint32_t *output, uint32_t *decrypt_buffer, int N_blocks,uint32_t *iv, uint32_t counter, uint32_t *sym_key){ /* Method to receive data */

    uint8_t char_key[16] = {0};
    uint32_t decrypted_aes[MAX_N_BLOCKS*4] = {0};
    uint32_t received_TAG[4] = {0};
    int i;
    uint32_t to_mac_length = (N_blocks-1)*4;
    uint32_t to_mac[(MAX_N_BLOCKS-1)*4] = {0};
    uint32_t mac_buffer[(MAX_N_BLOCKS-1)*4] = {0};
    uint32_t payload_length;
    uint32_t decrypt_counter = counter + N_blocks-1;

    /* convert sym_key to 8bit key */
    arr32_to_arr8(char_key,sym_key,4);

    /* decrypt received data */
    ctr_mode(decrypted_aes,decrypt_buffer,N_blocks,iv,decrypt_counter,char_key);
    for (i=0; i<4; i++){
        received_TAG[i] = decrypted_aes[i];
    }
    /* calculate TAG to compare with received tag */
    for (i=0; i < to_mac_length; i++){
        to_mac[i] = decrypted_aes[i+4];
    }

    ctr_mode(mac_buffer,to_mac,N_blocks-1,iv,counter,char_key);
    /* TAG is last block of mac_buffer */
    for (i=0; i<4;i++){
        if (mac_buffer[to_mac_length-4+ i] != received_TAG[i]){
            fprintf(stderr, "Received TAG differs from calculated TAG \r\n");
            printf("Went wrong at TAG part %d\r\n",i);
            printf("Calculated: %x \n",mac_buffer[to_mac_length-4+ i]);
            printf("but received: %x \r\n", received_TAG[i]);
            return 1;
        }
    }
    /* write message to output buffer */
    payload_length = decrypted_aes[4];
    /*printf("Message length in blocks: %x \n",payload_length);*/
    for (i=0; i<payload_length*4;i++){
        output[i] = decrypted_aes[i+5]; /* 4 x 32-bits for TAG, one 32-bit number for message_length*/
    }
    return 0;
}
