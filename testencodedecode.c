#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "globals.h"
#include "make_subbands.h"


int main (int argc, char *argv[])
{

  struct chunkTransmit chunk1;
  memset(&chunk1, 0, sizeof(struct chunkTransmit));
  for (int k=0;k<4;k++){
	  chunk1.stepsizes[k] = 200;
  }
  
  struct chunkReceive chunk2;
  memset(&chunk2, 0, sizeof(struct chunkReceive));
  for (int k=0;k<4;k++){
	  chunk2.stepsizes[k] = 200;
  }
  short d[5] = {500, 900, 1900,-590, 0};
  
  encode(d, (short)pow(2,15), &chunk1, 5, 1);
  
  printf("\n tssresult ");
  for (int i=0;i<5;i++){
    printf("%i; ",d[i]);
  }
  
  decode(d, (short)pow(2,15), &chunk2, 5, 1);
  
  printf("\n result ");
  for (int i=0;i<5;i++){
    printf("%i; ",d[i]);
  }
    
  return 0;
	
}
