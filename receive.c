void receive(short *buffer, struct chunk *chunk_left, struct chunk *chunk_right, short *input){
	
	short subband1L[5];
    short subband2L[5];
    short subband3L[5];
    short subband4L[5];
    short subband1R[5];
    short subband2R[5];
    short subband3R[5];
	short subband4R[5];
	short left[20];
	short right[20];
	short packedLeft[4];
	short packedRight[4];
	for (short i = 0 ; i < 3 ; i += 2){
		packedLeft[i] = left[i];
		packedRight[i] = right[i+1];
	}
	unpackBits(left,subband1L,subband2L,subband3L,subband4L);
	unpackBits(left,subband1L,subband2L,subband3L,subband4L);

	
	// subbands uit encoded buffer halen
	
	decode(subband1R, mu1, 5, chunk_right, 1);
	decode(subband2R, mu2, 3, chunk_right, 2);
    decode(subband3R, mu3, 2, chunk_right, 3);
    decode(subband4R, mu4, 2, chunk_right, 4);
    decode(subband1L, mu1, 5, chunk_right, 1);
    decode(subband2L, mu2, 3, chunk_right, 2);
    decode(subband3L, mu3, 2, chunk_right, 3);
	decode(subband4L, mu4, 2, chunk_right, 4);
	
	restore_subbands(right, subband1R, subband2R, subband3R, subband4R, chunk_right);
	restore_subbands(left, subband1L, subband2L, subband3L, subband4L, chunk_left);
	
	for (short i = 0 ; i < BUFFERSIZE ; i += 2){
		buffer[i] = left[i/2];
		buffer[i+1] = right[i/2];
	}
}