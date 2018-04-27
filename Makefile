LDFLAGS=-lm
CFLAGS=-Wall -ansi -pedantic-errors -g -O0 -std=c99

SBC: main.o wavpcm_io.o functions.o test.c aes.c format.c encryption.c montgomery2.c padding.c handshake.c global.c datagram.c master_fsm.c slave_fsm.c digest-info.c sha256.c
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@