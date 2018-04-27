#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wavpcm_io.h"
#include "global.h"
#include "math.h"
#include "functions.h"

#include "format.h"
#include "cryptomain.h"
#include "datagram.h"


/* This is the function that is called when the program starts. */
int main (int argc, char *argv[])
{
  /* Variable declarations. */
  struct wavpcm_input input;
  struct wavpcm_output output;
  short buffer[BUFFERSIZE]; 
  short reconstructedBuffer[BUFFERSIZE];
  int bufPos, bufIndex, read;
  int i, k;

  memset(&input, 0, sizeof(struct wavpcm_input));
  input.resource=INPUTWAVFILE;
  memset(&output, 0, sizeof(struct wavpcm_output));
  output.resource=OUTPUTWAVFILE;

  /* First open input file and parse header, */
  wavpcm_input_open (&input);
  /* and then use this same header configuration for the output file */
  wavpcm_output_copy_settings(&input, &output);
  wavpcm_output_open(&output);
  
  struct chunkTransmit chunkTransmitLeft;
  memset(&chunkTransmitLeft, 0, sizeof(struct chunkTransmit));
  struct chunkTransmit chunkTransmitRight;
  memset(&chunkTransmitRight, 0, sizeof(struct chunkTransmit));
  
  for (k=0;k<4;k++){
	  chunkTransmitLeft.stepsizes[k] = 100;
  }
  for (k=0;k<4;k++){
	  chunkTransmitRight.stepsizes[k] = 100;
  }
  
  struct chunkReceive chunkReceiveLeft;
  memset(&chunkReceiveLeft, 0, sizeof(struct chunkReceive));
  struct chunkReceive chunkReceiveRight;
  memset(&chunkReceiveRight, 0, sizeof(struct chunkReceive));
  
  for (k=0;k<4;k++){
	  chunkReceiveLeft.stepsizes[k] = 100;
  }
  for (k=0;k<4;k++){
	  chunkReceiveRight.stepsizes[k] = 100;
  }
  
  short subband1L[BUFFERSIZE8];
  short subband2L[BUFFERSIZE8];
  short subband3L[BUFFERSIZE8];
  short subband4L[BUFFERSIZE8];
  short subband1R[BUFFERSIZE8];
  short subband2R[BUFFERSIZE8];
  short subband3R[BUFFERSIZE8];
  short subband4R[BUFFERSIZE8];
  short resultL[4];
  short resultR[4];
  short resultCombined[8];
  
  
  
  /*bufPos expressed in temporal samples*/
  for (bufPos=0; bufPos<input.samplesAvailable; bufPos+=(BUFFERSIZE2)) {
    /* Try to read BUFFERSIZE samples (16 bits, pairwise identical if input is mono, interleaved if input is stereo)  */
    /* into buffer, with read the actual amount read (expressed in bytes! =  (2*read)/(channels * bitDepth/8) array elements)*/
    read = wavpcm_input_read (&input, buffer);
    
    
    long long int bufferL[BUFFERSIZE2];
    long long int bufferR[BUFFERSIZE2];
    
    for (i=0;i<BUFFERSIZE;i+=2){
      bufferL[i/2] = buffer[i];
      bufferR[i/2] = buffer[i+1];
    }

    
    /* transform buffer */
    
    
    make_subbands(bufferL, subband1L, subband2L, subband3L, subband4L, &chunkTransmitLeft);
    make_subbands(bufferR, subband1R, subband2R, subband3R, subband4R, &chunkTransmitRight);
    encode(subband1R, mu1, &chunkTransmitRight, 5,  1);
    encode(subband2R, mu2, &chunkTransmitRight, 3,  2);
    encode(subband3R, mu3, &chunkTransmitRight, 2,  3);
    encode(subband4R, mu4, &chunkTransmitRight, 2,  4);
    encode(subband1L, mu1, &chunkTransmitLeft,  5,  1);
    encode(subband2L, mu2, &chunkTransmitLeft,  3,  2);
    encode(subband3L, mu3, &chunkTransmitLeft,  2,  3);
    encode(subband4L, mu4, &chunkTransmitLeft,  2,  4);
    
    packBits(subband1L, subband2L, subband3L, subband4L, resultL);
    packBits(subband1R, subband2R, subband3R, subband4R, resultR);
    
    for (i = 0; i<8 ; i+=2){
	  resultCombined[i] = resultL[i/2];
	  resultCombined[i+1] = resultR[i/2];
    }
    
    
    arr16_to_arr32(input_buffer, resultCombined, 8);
    
    uint32_t iv[3] = {1,0};
    uint32_t counter = 0;
    uint32_t key[4] = {1,0};
    int N_blocks_send = 1;
    int N_blocks_receive = 3;
    uint32_t sending_buffer[16];
    short output_c[8];
    master_datagram(sending_buffer,input_buffer,N_blocks_send,iv,counter,key);
    slave_datagram(output_buffer,sending_buffer,N_blocks_receive,iv,counter,key);
    
    arr32_to_arr16(output_c, output_buffer, 4);
    
    if (memcmp(resultCombined, output_c, 8) != 0) {
        fprintf(stderr, "Datagram protocol failed\r\n");
        printf("Printing expected output:\r\n");
        /*printArray(resultCombined,8);*/
        printf("Printing received output:\r\n");
        /*printArray(output_c,8);*/
        abort();
    }
    else{
        printf("Data transmission correct! \r\n");
    }
    
    for (i = 0; i<8 ; i+=1){
	  resultCombined[i] = output_c[i];
    }
    
    for (i = 0; i<8 ; i+=2){
	  resultL[i/2] = resultCombined[i];
	  resultR[i/2] = resultCombined[i+1];
    }
    
    unpackBits(resultL, subband1L, subband2L, subband3L, subband4L);
    unpackBits(resultR, subband1R, subband2R, subband3R, subband4R);
 
    decode(subband1R, mu1, &chunkReceiveRight, 5,  1);
    decode(subband2R, mu2, &chunkReceiveRight, 3,  2);
    decode(subband3R, mu3, &chunkReceiveRight, 2,  3);
    decode(subband4R, mu4, &chunkReceiveRight, 2,  4);
    decode(subband1L, mu1, &chunkReceiveLeft,  5,  1);
    decode(subband2L, mu2, &chunkReceiveLeft,  3,  2);
    decode(subband3L, mu3, &chunkReceiveLeft,  2,  3);
    decode(subband4L, mu4, &chunkReceiveLeft,  2,  4);
    restore_signal(bufferL, subband1L, subband2L, subband3L, subband4L, &chunkReceiveLeft);
    restore_signal(bufferR, subband1R, subband2R, subband3R, subband4R, &chunkReceiveRight);
    /* inverse transform buffer */
	
    
    for (i=0;i<BUFFERSIZE;i+=2){
      buffer[i] = bufferL[i/2];
      buffer[i+1] = bufferL[i/2];
    }
	
	
    for (bufIndex=0; bufIndex<BUFFERSIZE; bufIndex++)
      reconstructedBuffer[bufIndex]=buffer[bufIndex];

    /* dump reconstructed output */
    wavpcm_output_write (&output, reconstructedBuffer, read);
  }

  /* finalize output (write header) and close */
  wavpcm_output_close (&output);  

  /* Return successful exit code. */
  return 0;
}
