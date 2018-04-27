struct chunkTransmit{
  short conv10x1[19];
  short conv10x2_19[19];
  short conv10x2_1;
	
  long long int conv16x1_1[20];
  long long int conv16x2_20_1[20];
  long long int conv16x2_1_1;
	
  long long int conv16x1_2[20];
  long long int conv16x2_20_2[20];
  long long int conv16x2_1_2;
  
  
  short predictions[4];
  short stepsizes[4];
  short deltas[80];
};
struct chunkReceive{
  long long int conv10x1[19];
  long long int conv10x2_19[19];
  long long int conv10x2_1;
	
  long long int conv16x1_1[20];
  long long int conv16x2_20_1[20];
	
  long long int conv16x1_2[20];
  long long int conv16x2_20_2[20];
  
  short predictions[4];
  short stepsizes[4];
  short deltas[80];
};


void make_subbands(long long int *signal, short *subband1, short *subband2, short *subband3, short *subband4, struct chunkTransmit *chunk1);

void restore_signal(long long int *signal, short *subband1, short *subband2, short *subband3, short *subband4, struct chunkReceive *chunk1);

void add(long long int *a, long long int *b, long long int *res, short SIZE);

void convolution10(long long int *filter, short *conv10x1_19, long long int *result);

void convolution10_SYNTH(long long int *filter, long long int *conv10x1_19, long long int *result);

void convolution16(long long int *filter, long long int *conv10x1_20, long long int *result);

void subtract(long long int *a, long long int *b, long long int *res, int SIZE);

void updateChunkX1_10(struct chunkTransmit *chunk1, long long int *signal);

void updateChunkX2_10(struct chunkTransmit *chunk1, long long int *signal);

void updateChunkX1_16_1(struct chunkTransmit *chunk1, long long int *signal);

void updateChunkX1_16_2(struct chunkTransmit *chunk1, long long int *signal);

void updateChunkX2_16_1(struct chunkTransmit *chunk1, long long int *signal);

void updateChunkX2_16_2(struct chunkTransmit *chunk1, long long int *signal);

void updateChunkX2_10_SYNTH(struct chunkReceive *chunk1, long long int *signal);

void updateChunkX2_16_1_SYNTH(struct chunkReceive *chunk1, long long int *signal);

void updateChunkX2_16_2_SYNTH(struct chunkReceive *chunk1, long long int *signal);

void updateChunkX1_16_1_SYNTH(struct chunkReceive *chunk1, long long int *signal);

void updateChunkX1_16_2_SYNTH(struct chunkReceive *chunk1, long long int *signal);



short calculateStandardDeviation(short *values, short SIZE);

short updateStepsize(short *values, short nbBits, short SIZE);

short quantize(short d, short stepsize, short nbBits);

short dequantize(short d, short stepsize);

void encode(short *subband, short mu,  struct chunkTransmit *chunk, short nbBits,short subbandNr);

void decode(short *subband, short mu, struct chunkReceive *chunk, short nbBits, short subbandNr);

void packBits(short *subband1, short *subband2, short *subband3, short *subband4, short *result);

void unpackBits(short *input, short *subband1, short *subband2, short *subband3, short *subband4);