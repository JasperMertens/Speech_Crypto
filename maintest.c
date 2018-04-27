#include "format.h"
#include <stdio.h>

int main (int argc, char *argv[])
{
  short array_signed[4] = {-20100, -1678, -88, -300};
  uint32_t array_unsigned[2];
  short result[4];
  int i;
  
  printf("ARRAY_SIGNED: \n");
  for (i=0;i<4;i++){
    printf("%x \n", array_signed[i]);
  }
  
  arr16_to_arr32(array_unsigned, array_signed, 4);
  
  printf("ARRAY_UNSIGNED: \n");
  for (i=0;i<2;i++){
    printf("%x \n", array_unsigned[i]);
  }
  
  arr32_to_arr16(result, array_unsigned, 2);
  
  printf("RESULT: \n");
  for (i=0;i<4;i++){
    printf("%i \n", result[i]);
  }
  
  /* Return successful exit code. */
  return 0;
}