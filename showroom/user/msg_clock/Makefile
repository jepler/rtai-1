prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)

all: clock keybrd screen

clock: clock.o cmdclk.o cmdcrn.o dispclk.o
	$(CC) -o $@ $^ $(LXRT_LDFLAGS)

%.o: %.c
	$(CC) $(LXRT_CFLAGS) -c $<  -llxrt

keybrd: keybrd.c
	$(CC) $(LXRT_CFLAGS) -o $@ $< $(LXRT_LDFLAGS)

screen: screen.c
	$(CC) $(LXRT_CFLAGS) -o $@ $< $(LXRT_LDFLAGS)

clean:
	rm -f *.o clock keybrd screen

.PHONY: clean
