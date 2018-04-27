#include <stdio.h>
#include "math.h"
#include "global.h"
#include "functions.h"


void add(long long int *a, long long int *b, long long int *res, short SIZE){
	int i;
	for (i=0 ; i<SIZE ; i++){
	  res[i] = (a[i] + b[i]);
  }
}
 
 
 
void convolution10(long long int *filter, short *conv10x1_19, long long int *result){
  int m,n;
  const int start = filterLength1 - 1; 
  const int size = filterLength1+(BUFFERSIZE4)-1; 
 
  long long int res = 0;
  
  for(n=0; n<size-start; n++){
    for(m=0; m<filterLength1 ; m++){
      res = res + (filter[m]) * (conv10x1_19[n-m+start]);
    } 
    result[n] = res/(32786);
    res = 0;
  }
}

void convolution10_SYNTH(long long int *filter, long long int *conv10x1_19, long long int *result){

  int m,n;
  const int start = filterLength1 - 1; 
  const int size = filterLength1+(BUFFERSIZE4)-1;
 
  long long int res = 0;

  for(n=0; n<size-start; n++){
    for(m=0; m<filterLength1 ; m++){
      res = res + (filter[m]) * (conv10x1_19[n-m+start]);
    } 
    result[n] = res/(32786);
    res = 0;
  }
}


void convolution16(long long int *filter, long long int *conv10x1_20, long long int *result){
	
  int m,n;
  const int start = filterLength2 - 1;
  const int size = filterLength2+(BUFFERSIZE8)-1; 
 
  long long int res = 0;
  
  for(n=0; n<size-start; n++){
    for(m=0; m<filterLength2 ; m++){
      res = res + (filter[m]) * (conv10x1_20[n-m+start]);
    }  
    result[n] = res/(32786);
    res = 0;
  }
}



void subtract(long long int *a, long long int *b, long long int *res, int SIZE){
	int i;
	for (i=0 ; i<SIZE ; i++){
	  res[i] = (a[i] - b[i]);
  }
}



void updateChunkX1_10(struct chunkTransmit *chunk1, long long int *signal){
  int k,i;
  short conv10x1[19];
  
  for (k=0; k<19; k++){
    conv10x1[k]  = chunk1->conv10x1[k];
  }
  
  for (i=0; i<filterLength1-1; i++){
	chunk1->conv10x1[i] = conv10x1[i+filterLength1];  /* posities 10..18 naar 0..8 */
	chunk1->conv10x1[i+filterLength1-1] = signal[i]; /* nieuwe waarden posities 9..17 */
  } 
  chunk1->conv10x1[2*filterLength1-2] = signal[filterLength1-1];/*nieuwe waarde positie 18*/
}



void updateChunkX2_10(struct chunkTransmit *chunk1, long long int *signal){
  int k,i;
  short conv10x2_19[19];
  for (k=0; k<19; k++){
    conv10x2_19[k]  = chunk1->conv10x2_19[k];
  }
  
  
  for (i=0; i<filterLength1-1; i++){
	chunk1->conv10x2_19[i] = conv10x2_19[i+filterLength1];  /* posities 10..18 naar 0..8*/
	chunk1->conv10x2_19[i+filterLength1] = signal[i]; /* nieuwe waarden posities 10..18*/
  } 
  chunk1->conv10x2_19[filterLength1-1] = chunk1->conv10x2_1; /* de oude aparte (conv10x2_1) komt op positie 9*/
  chunk1->conv10x2_1 = signal[filterLength1-1];/* nieuwe waarde conv10x2_1;*/
  
}


void updateChunkX1_16_1(struct chunkTransmit *chunk1, long long int *signal){

  int k,i;
  long long int conv16x1_1[20];
  
  for (k=0; k<20; k++){
    conv16x1_1[k]  = chunk1->conv16x1_1[k];
  }
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x1_1[i] = conv16x1_1[i+BUFFERSIZE8];  /*posities 5..19 naar 0..14*/
  } 
  for (i=0; i<BUFFERSIZE8; i++){
	chunk1->conv16x1_1[i+filterLength2-1] = signal[i]; /* nieuwe waarden posities 15..19*/
  } 
}




void updateChunkX1_16_2(struct chunkTransmit *chunk1, long long int *signal){
  int k,i;
  long long int conv16x1_2[20];
  
  for (k=0; k<20; k++){
    conv16x1_2[k]  = chunk1->conv16x1_2[k];
  }
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x1_2[i] = conv16x1_2[i+BUFFERSIZE8];  /* posities 5..19 naar 0..14 */
  } 
  for (i=0; i<BUFFERSIZE8; i++){
	chunk1->conv16x1_2[i+filterLength2-1] = signal[i]; /* nieuwe waarden posities 15..19 */
  } 
}



void updateChunkX2_16_1(struct chunkTransmit *chunk1, long long int *signal){

  int k,i;
  long long int conv16x2_20_1[20];
  for (k=0; k<20; k++){
    conv16x2_20_1[k]  = chunk1->conv16x2_20_1[k];
  }
  
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x2_20_1[i] = conv16x2_20_1[i+BUFFERSIZE8];  /* posities 5..19 naar 0..14*/
  } 
  for (i=0; i<BUFFERSIZE8-1; i++){
	chunk1->conv16x2_20_1[i+filterLength2] = signal[i]; /* nieuwe waarden posities 16..19*/
  } 
  chunk1->conv16x2_20_1[filterLength2-1] = chunk1->conv16x2_1_1; /* de oude aparte (conv16x2_1_1) komt op positie 15*/
  chunk1->conv16x2_1_1 = signal[BUFFERSIZE8-1];/* nieuwe waarde conv16x2_1_1;*/
  
}



void updateChunkX2_16_2(struct chunkTransmit *chunk1, long long int *signal){
  
  int k,i;
  long long int conv16x2_20_2[20];
  for (k=0; k<20; k++){
    conv16x2_20_2[k]  = chunk1->conv16x2_20_2[k];
  }
  
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x2_20_2[i] = conv16x2_20_2[i+BUFFERSIZE8];  
  } 
  for (i=0; i<BUFFERSIZE8-1; i++){
	chunk1->conv16x2_20_2[i+filterLength2] = signal[i]; 
  } 
  chunk1->conv16x2_20_2[filterLength2-1] = chunk1->conv16x2_1_2; 
  chunk1->conv16x2_1_2 = signal[BUFFERSIZE8-1];
  
}


void updateChunkX1_10_SYNTH(struct chunkReceive *chunk1, long long int *signal){
  
  int k,i;
  long long int conv10x1[19];
  
  for (k=0; k<19; k++){
    conv10x1[k]  = chunk1->conv10x1[k];
  }
  
  for (i=0; i<filterLength1-1; i++){
	chunk1->conv10x1[i] = conv10x1[i+filterLength1];  
	chunk1->conv10x1[i+filterLength1-1] = signal[i]; 
  } 
  chunk1->conv10x1[2*filterLength1-2] = signal[filterLength1-1];
}

void updateChunkX2_10_SYNTH(struct chunkReceive *chunk1, long long int *signal){
  
  int k,i;
  long long int conv10x2_19[19];
  
  for (k=0; k<19; k++){
    conv10x2_19[k]  = chunk1->conv10x2_19[k];
  }
  
  for (i=0; i<filterLength1-1; i++){
	chunk1->conv10x2_19[i] = conv10x2_19[i+filterLength1];  
	chunk1->conv10x2_19[i+filterLength1-1] = signal[i];
  } 
  chunk1->conv10x2_19[2*filterLength1-2] = signal[filterLength1-1];
}


void updateChunkX2_16_1_SYNTH(struct chunkReceive *chunk1, long long int *signal){

  int k,i;
  long long int conv16x2_20_1[20];
  
  for (k=0; k<20; k++){
    conv16x2_20_1[k]  = chunk1->conv16x2_20_1[k];
  }
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x2_20_1[i] = conv16x2_20_1[i+BUFFERSIZE8]; 
  } 
  for (i=0; i<BUFFERSIZE8; i++){
	chunk1->conv16x2_20_1[i+filterLength2-1] = signal[i]; 
  } 
}


void updateChunkX2_16_2_SYNTH(struct chunkReceive *chunk1, long long int *signal){
  int k,i;
  long long int conv16x2_20_2[20];
  
  for (k=0; k<20; k++){
    conv16x2_20_2[k]  = chunk1->conv16x2_20_2[k];
  }
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x2_20_2[i] = conv16x2_20_2[i+BUFFERSIZE8];  
  } 
  for (i=0; i<BUFFERSIZE8; i++){
	chunk1->conv16x2_20_2[i+filterLength2-1] = signal[i];
  } 
}

void updateChunkX1_16_1_SYNTH(struct chunkReceive *chunk1, long long int *signal){

  int k,i;
  long long int conv16x1_1[20];
  
  for (k=0; k<20; k++){
    conv16x1_1[k]  = chunk1->conv16x1_1[k];
  }
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x1_1[i] = conv16x1_1[i+BUFFERSIZE8];  
  } 
  for (i=0; i<BUFFERSIZE8; i++){
	chunk1->conv16x1_1[i+filterLength2-1] = signal[i]; 
  } 
}

void updateChunkX1_16_2_SYNTH(struct chunkReceive *chunk1, long long int *signal){

  int k,i;
  long long int conv16x1_2[20];
  
  for (k=0; k<20; k++){
    conv16x1_2[k]  = chunk1->conv16x1_2[k];
  }
  
  for (i=0; i<filterLength2-1; i++){
	chunk1->conv16x1_2[i] = conv16x1_2[i+BUFFERSIZE8]; 
  } 
  for (i=0; i<BUFFERSIZE8; i++){
	chunk1->conv16x1_2[i+filterLength2-1] = signal[i];
  } 
}



void make_subbands(long long int *signal, short *subband1, short *subband2, short *subband3, short *subband4, struct chunkTransmit *chunk1){  

  long long int filt1[filterLength1] = {3, 16, -69, -225, 3020, 15830, -2836, 758, -118, 3};
  long long int filt2[filterLength2] = {2, 6, 14, -38, 19, 176, -897, 3851, 15477, -3229, 1488,-678, 263, -78, 15, -10};
  
  int i;
  short pos;
  
  long long int signal_x1[BUFFERSIZE4]; /*downsampled version with 1 shift*/
  long long int signal_x2[BUFFERSIZE4]; /* downsampled version without shift*/
  long long int u1[BUFFERSIZE4];
  long long int u2[BUFFERSIZE4];
  
  long long int temporalSubbandLow[BUFFERSIZE4];
  long long int temporalSubbandHigh[BUFFERSIZE4];
  
  long long int signal_x1_tSL[BUFFERSIZE8];
  long long int signal_x2_tSL[BUFFERSIZE8];
  long long int signal_x1_tSH[BUFFERSIZE8];
  long long int signal_x2_tSH[BUFFERSIZE8];
  long long int u1_tSL[BUFFERSIZE8];
  long long int u2_tSL[BUFFERSIZE8];
  long long int u1_tSH[BUFFERSIZE8];
  long long int u2_tSH[BUFFERSIZE8];
  
  long long int result1[BUFFERSIZE8];
  long long int result2[BUFFERSIZE8];
  
  
  /* LEVEL 1 
   splits ingangssignaal in 2 downsampled versions */
  for (pos = 0 ; pos < BUFFERSIZE2 ; pos += 2){
	  signal_x2[pos/2] = signal[pos+1];
	  signal_x1[pos/2] = signal[pos];
  }
  
  
  updateChunkX1_10(chunk1, signal_x1);	
  updateChunkX2_10(chunk1, signal_x2);	
 
  convolution10(filt1, chunk1->conv10x1, u1);
  convolution10(filt1, chunk1->conv10x2_19, u2);
  

  add(u1, u2, temporalSubbandLow, BUFFERSIZE4);
  subtract(u1, u2, temporalSubbandHigh, BUFFERSIZE4);
  
  
  
  
  /* LEVEL 2 */
  
  for (pos = 0 ; pos < BUFFERSIZE4 ; pos += 2){
	  signal_x2_tSL[pos/2] = temporalSubbandLow[pos+1];
	  signal_x1_tSL[pos/2] = temporalSubbandLow[pos];
  }
  
  
  updateChunkX1_16_1(chunk1, signal_x1_tSL);	
  updateChunkX2_16_1(chunk1, signal_x2_tSL);	
 
  
 
  convolution16(filt2, chunk1->conv16x1_1, u1_tSL);
  convolution16(filt2, chunk1->conv16x2_20_1, u2_tSL);


  
  add(u1_tSL, u2_tSL, result1, BUFFERSIZE8);
  subtract(u1_tSL, u2_tSL, result2, BUFFERSIZE8);
  
  
  for (i=0;i<BUFFERSIZE8;i++){
    subband1[i] = (short) (result1[i]);
    subband2[i] = (short) (result2[i]);
  }
  
  
  
  
  
  
  
  for (pos = 0 ; pos < BUFFERSIZE4 ; pos += 2){
	  signal_x2_tSH[pos/2] = temporalSubbandHigh[pos+1];
	  signal_x1_tSH[pos/2] = temporalSubbandHigh[pos];
  }
  
  
  updateChunkX1_16_2(chunk1, signal_x1_tSH);	
  updateChunkX2_16_2(chunk1, signal_x2_tSH);	
 
 
  convolution16(filt2, chunk1->conv16x1_2, u1_tSH);
  convolution16(filt2, chunk1->conv16x2_20_2, u2_tSH);
 
  
  add(u1_tSH, u2_tSH, result1, BUFFERSIZE8);
  subtract(u1_tSH, u2_tSH, result2, BUFFERSIZE8);
  
  
  for (i=0;i<BUFFERSIZE8;i++){
    subband3[i] = (short) (result1[i]);
    subband4[i] = (short) (result2[i]);
  }
 
  
}




void restore_signal(long long int *signal, short *subband1, short *subband2, short *subband3, short *subband4, struct chunkReceive *chunk1){  

  long long int filt1[filterLength1] = {3, 16, -69, -225, 3020, 15830, -2836, 758, -118, 3};
  long long int filt2[filterLength2] = {2, 6, 14, -38, 19, 176, -897, 3851, 15477, -3229, 1488,-678, 263, -78, 15, -10};
  int k, i;

  long long int sub1[BUFFERSIZE8];
  long long int sub2[BUFFERSIZE8];
  long long int sub3[BUFFERSIZE8];
  long long int sub4[BUFFERSIZE8];
  
  for (k=0; k<BUFFERSIZE8; k++){
    sub1[k] = (long long int) subband1[k];
  }
  for (k=0; k<BUFFERSIZE8; k++){
    sub2[k] = (long long int) subband2[k];
  }
  for (k=0; k<BUFFERSIZE8; k++){
    sub3[k] = (long long int) subband3[k];
  }
  for (k=0; k<BUFFERSIZE8; k++){
    sub4[k] = (long long int) subband4[k];
  }
   
  long long int u1_tSL[BUFFERSIZE8];
  long long int u2_tSL[BUFFERSIZE8];
  long long int u1_tSH[BUFFERSIZE8];
  long long int u2_tSH[BUFFERSIZE8];
  
  long long int v1_tSL[BUFFERSIZE8];
  long long int v2_tSL[BUFFERSIZE8];
  long long int v1_tSH[BUFFERSIZE8];
  long long int v2_tSH[BUFFERSIZE8];
  
  long long int temporalSubbandLow[BUFFERSIZE4];
  long long int temporalSubbandHigh[BUFFERSIZE4];
  
  long long int u1[BUFFERSIZE4];
  long long int u2[BUFFERSIZE4];
  
  long long int v1[BUFFERSIZE4];
  long long int v2[BUFFERSIZE4];
  
  /* LEVEL 2 (low)*/
  
  
  
  add(sub1, sub2, u1_tSL, BUFFERSIZE8);
  subtract(sub1, sub2, u2_tSL, BUFFERSIZE8);
  
 
  
  updateChunkX1_16_1_SYNTH(chunk1, u1_tSL);	
  updateChunkX2_16_1_SYNTH(chunk1, u2_tSL);	
  
 
 
  convolution16(filt2, chunk1->conv16x1_1, v1_tSL);
  convolution16(filt2, chunk1->conv16x2_20_1, v2_tSL);
 

  for (i=0; i<10;i+=2){
	  temporalSubbandLow[i] = v2_tSL[i/2];
	  temporalSubbandLow[i+1] = v1_tSL[i/2];
  }
  
  
  
  
  
  /* LEVEL 2 (high)*/
  
  add(sub3, sub4, u1_tSH, BUFFERSIZE8);
  subtract(sub3, sub4, u2_tSH, BUFFERSIZE8);

  updateChunkX1_16_2_SYNTH(chunk1, u1_tSH);	
  updateChunkX2_16_2_SYNTH(chunk1, u2_tSH);	
  

  convolution16(filt2, chunk1->conv16x1_2, v1_tSH);
  convolution16(filt2, chunk1->conv16x2_20_2, v2_tSH);

  
  for (i=0; i<10;i+=2){
    temporalSubbandHigh[i] = v2_tSH[i/2];
    temporalSubbandHigh[i+1] = v1_tSH[i/2];
  }
  
 
  
  /* LEVEL 1 */
  
  add(temporalSubbandLow, temporalSubbandHigh, u1, BUFFERSIZE4);
  subtract(temporalSubbandLow, temporalSubbandHigh, u2, BUFFERSIZE4);
  
 
  
  updateChunkX1_10_SYNTH(chunk1, u1);	
  updateChunkX2_10_SYNTH(chunk1, u2);	
  

 
  
  convolution10_SYNTH(filt1, chunk1->conv10x1, v1);
  convolution10_SYNTH(filt1, chunk1->conv10x2_19, v2);
  
  
  for (i=0; i<20;i+=2){
	  signal[i] = v2[i/2];
	  signal[i+1] = v1[i/2];
  }
   
}

short calculateStandardDeviation(short *values, short SIZE){
	long long int mean = 0;
	int i;
	long long int standardDeviation = 0;
	for (i=0; i<SIZE ; i++){
		mean = mean+values[i];
	}
	mean = mean/SIZE;

	for (i=0; i<SIZE ; i++){
		standardDeviation = standardDeviation + (values[i]-mean)*(values[i]-mean);
	}
	standardDeviation = standardDeviation * 1024;
	standardDeviation = sqrt(standardDeviation/(SIZE-1));
	standardDeviation = standardDeviation/ (32);
	return standardDeviation;
}


short quantize(short d, short stepsize, short nbBits){
	short a = (1<<nbBits);
	short highestValue = (a/2-1)*stepsize + stepsize/2;
	short lowestValue = (-a/2)*stepsize - stepsize/2;
	short value;
	short i = -(a)/2;
	short result = (a)/2-1;
	for (value = lowestValue + stepsize; value < highestValue; value = value + stepsize){
		if(d < value){
			return i;
		}
		i = i + 1;
	}
	return result;
}

short updateStepsize(short *values, short nbBits, short SIZE){
  short standardDeviation = calculateStandardDeviation(values, SIZE);
  long long int phi;
  if (nbBits == 2){
    phi =  32627; /* 0.9957*2^15*/
  }
  if (nbBits == 3){
    phi = 19202; /* 0.5860 *2^15*/
  }
  if (nbBits == 5){
    phi = 6164; /* 0.1881*2^15*/
  }

  short stepsize = (standardDeviation*phi)/(32786);
  if (stepsize == 0){
    stepsize = 1;
  }
  return stepsize;
}


short dequantize(short d, short stepsize){
  short result = d*stepsize;
  return result;
}


void encode(short *subband, short mu, struct chunkTransmit *chunk, short nbBits, short subbandNr){
	  
	  /* get chunk-values*/
	  short prediction = chunk->predictions[subbandNr-1];
	  short stepsize = chunk->stepsizes[subbandNr-1];
	  short deltas[nbDeltas];
	  int i;
	  short q;
	  
	  /* deltas voor berekenen standaarddeviatie vanuit chunk in deltas steken. (afhankelijk van nummer van subband)*/
	  for (i = 0; i<nbDeltas ; i++){
		deltas[i] = chunk->deltas[nbDeltas*(subbandNr-1)+i];
	  }
	  short d;
	
	  
	  for(i = 0; i<BUFFERSIZE8; i++){
		  d = subband[i] - prediction; /* verschil tussen echte waarde en predicti*/
		  /* Test results*/
		  subband[i] = quantize(d, stepsize, nbBits); /* kwantiseren van waarde die doorgestuurd wordt */
		  for (q = 0 ; q < nbDeltas-1 ; q++){
			  /* deltas opschuiven en nieuwe inladen*/
			  deltas[q] = deltas[q+1];
		  }
		  deltas[nbDeltas-1] = dequantize(subband[i], stepsize);
		  printf("\n test \n");
		  prediction = mu*(prediction + dequantize(subband[i], stepsize)) / (32786);
		  stepsize = updateStepsize(deltas, nbBits, nbDeltas); 
		  
	  }
	  
	  /* chunk variabelen terug schrijven*/
	  chunk->predictions[subbandNr-1] = prediction;
	  chunk->stepsizes[subbandNr-1] = stepsize;
	  for (i = 0 ; i< nbDeltas; i++){
		  chunk->deltas[nbDeltas*(subbandNr-1)+i] = deltas[i];
	  }
	  
  
}


void decode(short *subband, short mu, struct chunkReceive *chunk, short nbBits, short subbandNr){
	
	
	short d;
	short stepsize = chunk->stepsizes[subbandNr-1];
	short prediction = chunk->predictions[subbandNr-1];
	short deltas[nbDeltas];
	int i,j;
	
	
	for (i = 0; i<nbDeltas ; i++){
		  deltas[i] = chunk->deltas[nbDeltas*(subbandNr-1)+i];
	  }
	
	for(i = 0; i<BUFFERSIZE8; i++){
		printf("\n test \n");
		d = dequantize(subband[i], stepsize);
		subband[i] = prediction + d;
		prediction = mu*subband[i]/(32786);
		
		/* deltas updaten met gedekwantizeerde input */
		for (j = 0 ; j < nbDeltas-1 ; j++){
			  deltas[j] = deltas[j+1];
		  }
		deltas[nbDeltas-1] = d;
	  
	        stepsize = updateStepsize(deltas, nbBits, nbDeltas);
	}
	
	
	chunk->stepsizes[subbandNr-1] = stepsize;
	chunk->predictions[subbandNr-1] = prediction;
	for (i = 0 ; i< nbDeltas; i++){
		  chunk->deltas[nbDeltas*(subbandNr-1)+i] = deltas[i];
	  }
	
}

void packBits(short *subband1, short *subband2, short *subband3, short *subband4, short *result){
	
	int i;
	unsigned short subband1unsigned[5];
	unsigned short subband2unsigned[5];
	unsigned short subband3unsigned[5];
	unsigned short subband4unsigned[5];
	
	for (i=0 ; i<5 ; i++){
		subband1unsigned[i] = subband1[i] + 16;
		subband2unsigned[i] = subband2[i] + 4;
		subband3unsigned[i] = subband3[i] + 2;
		subband4unsigned[i] = subband4[i] + 2;
	}	
	

	
	result[0] = (subband2unsigned[4] + subband2unsigned[3]*8 + subband2unsigned[2]*64 + subband2unsigned[1]*512 + subband2unsigned[0]*4096) - 32786;
	result[1] = (subband1unsigned[2] + subband1unsigned[1]*32 + subband1unsigned[0]*1024) - 32786;
	result[2] = (subband3unsigned[2] + subband3unsigned[1]*4 + subband3unsigned[0]*16 + subband1unsigned[4]*64 + subband1unsigned[3]*2048) - 32786;
	result[3] = (subband4unsigned[4] + subband4unsigned[3]*4 + subband4unsigned[2]*16 + subband4unsigned[1]*64 + subband4unsigned[0]*256 + subband3unsigned[4]*1024 + subband3unsigned[3] * 4096) - 32786;
  
	
}

void unpackBits(short *input, short *subband1, short *subband2, short *subband3, short *subband4){
  
  unsigned short unsignedInput[4];	
  int i;
  unsigned short last5bits = 31;
  unsigned short last3bits = 7;	
  unsigned short last2bits = 3;
  
  for(i = 0 ; i<4 ; i++){
    unsignedInput[i] = input[i] + 32786;
  }
  

 
  subband1[0] = (unsignedInput[1] >> 10) - 16;
  subband1[1] = ((unsigned short)(unsignedInput[1] >> 5) & last5bits) - 16;
  subband1[2] = ((unsignedInput[1]) & last5bits) - 16;
  subband1[3] = ((unsignedInput[2] >> 11)) - 16;
  subband1[4] = ((unsigned short)(unsignedInput[2] >> 6) & last5bits) - 16;
  
  subband2[0] = (unsignedInput[0] >> 12) - 4;
  subband2[1] = ((unsigned short)(unsignedInput[0] >> 9) & last3bits) - 4;
  subband2[2] = ((unsigned short)(unsignedInput[0] >> 6) & last3bits) - 4;
  subband2[3] = ((unsigned short)(unsignedInput[0] >> 3) & last3bits) - 4;
  subband2[4] = ((unsignedInput[0]) & last3bits) - 4;
  
  subband3[0] = ((unsigned short)(unsignedInput[2] >> 4) & last2bits) - 2;
  subband3[1] = ((unsigned short)(unsignedInput[2] >> 2) & last2bits) - 2;
  subband3[2] = ((unsignedInput[2]) & last2bits) - 2;
  subband3[3] = ((unsignedInput[3] >> 12)) - 2;
  subband3[4] = ((unsigned short)(unsignedInput[3] >> 10) & last2bits) - 2;
  
  subband4[0] = ((unsigned short)(unsignedInput[3] >> 8) & last2bits) - 2;
  subband4[1] = ((unsigned short)(unsignedInput[3] >> 6) & last2bits) - 2;
  subband4[2] = ((unsigned short)(unsignedInput[3] >> 4) & last2bits) - 2;
  subband4[3] = ((unsigned short)(unsignedInput[3] >> 2) & last2bits) - 2;
  subband4[4] = ((unsignedInput[3]) & last2bits) - 2;

}

