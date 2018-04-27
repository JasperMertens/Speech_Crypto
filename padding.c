/* padding.c */

#include <stdint.h>
#include <stdio.h>

typedef uint32_t u32;
typedef uint8_t u8;
# define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
# define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }

void addPadding(uint32_t *result, uint32_t* input, uint32_t input_byte_size, uint32_t output_byte_size) {
    uint32_t block_copy_length = input_byte_size/4;
    uint32_t bytes_copy_length = input_byte_size%4;
    uint8_t nb_padbytes = (uint8_t) (output_byte_size - input_byte_size);
    int i;
    uint32_t *in_pt = input;
    uint32_t *res_pt = result;
    uint8_t pad_string[4];
    for (i=0; i<block_copy_length; i++) {
        result[i] = input[i];
        in_pt++;
        res_pt++;
    }
    if (bytes_copy_length != 0) {
        uint8_t temp[4];
        PUTU32(temp,*in_pt);
        for (i=bytes_copy_length; i<4; i++) {
            temp[i] = nb_padbytes;
        }
        res_pt[0] = GETU32(temp);
        res_pt++;
    }
    pad_string[0] = nb_padbytes;
    pad_string[1] = nb_padbytes;
    pad_string[2] = nb_padbytes;
    pad_string[4] = nb_padbytes;
    for (i=0; i<nb_padbytes-4+bytes_copy_length; i++) {
        res_pt[i] = GETU32(pad_string);
    }

}

/* Both result and output_size are outputs of removePadding */
void removePadding(uint32_t *result, uint32_t output_size, uint32_t *input, uint32_t input_size);
