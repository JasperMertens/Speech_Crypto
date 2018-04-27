void transmit(short *buffer, struct chunk *chunk_left, struct chunk *chunk_right, short *result){

	
	short left[BUFFERSIZE/2];
	short right[BUFFERSIZE/2];
	short subband1L[BUFFERSIZE/8];
	short subband2L[BUFFERSIZE/8];
	short subband3L[BUFFERSIZE/8];
	short subband4L[BUFFERSIZE/8];
	short subband1R[BUFFERSIZE/8];
	short subband2R[BUFFERSIZE/8];
	short subband3R[BUFFERSIZE/8];
	short subband4R[BUFFERSIZE/8];
	short packedRight[4];
	short packedLeft[4];
	
	for (pos = 0 ; pos < BUFFERSIZE/2 ; pos += 2){
	  left[pos/2] = buffer[pos];
	  right[pos/2] = buffer[pos+1];
    }
	
	//ANALYSIS
	make_subbands(right, subband1R, subband2R, subband3R, subband4R, chunk_right);
	make_subbands(left, subband1L, subband2L, subband3L, subband4L, chunk_left);
	
	//ENCODING
    encode(subband1R, mu1, 5, chunk_right, 1);
	encode(subband2R, mu2, 3, chunk_right, 2);
    encode(subband3R, mu3, 2, chunk_right, 3);
    encode(subband4R, mu4, 2, chunk_right, 4);
    encode(subband1L, mu1, 5, chunk_right, 1);
    encode(subband2L, mu2, 3, chunk_right, 2);
    encode(subband3L, mu3, 2, chunk_right, 3);
	encode(subband4L, mu4, 2, chunk_right, 4);
	
	packBits(subband1R,subband2R,subband3R,subband4R,packedRight);
	packBits(subband1L,subband2L,subband3L,subband4L,packedLeft);
	
	for (int i = 0 ; i<4 ; i++){
			result[i] = packedLeft[i];
			result[i+4] = packedRight[i];
	}
	
}