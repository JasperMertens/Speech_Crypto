#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "math.h"



int main (int argc, char *argv[]){
  
  int i;
  
  short array_signed[4] = {20100, 33, 150, 999};
  uint32_t array_unsigned[4];
  
  printf("array_signed: \n");
  for (i=0;i<4;i++){
    printf("%i \n",array_signed[i]);
  }
  
  for (i=0;i<4;i++){
    array_unsigned[i] = (uint32_t)(array_signed[i]);
  }
  
  for (i=0;i<4;i++){
    array_signed[i] = (short)(array_unsigned[i]);
  }
  
  printf("array_signed: \n");
  for (i=0;i<4;i++){
    printf("%i \n",array_signed[i]);
  }

  /* Return successful exit code. */
  return 0;

}