CC?=gcc
CFLAGS+=-Wall -pedantic -std=c99 -D_BSD_SOURCE
LDFLAGS+=-lImlib2
PROG=icat
MODS=icat.o

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo unknown')

ifeq ($(uname_S),Darwin)
	CFLAGS:=$(CFLAGS) -I/opt/X11/include
endif

all: $(PROG)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROG): $(MODS)
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	-rm -f $(PROG) $(MODS)



