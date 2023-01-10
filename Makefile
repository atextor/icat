CC?=gcc
CFLAGS+=-Wall -pedantic -std=c99 -D_DEFAULT_SOURCE
LDFLAGS+=
PROG=icat
MODS=icat.o

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo unknown')

ifeq ($(uname_S),Darwin)
	CFLAGS:=$(CFLAGS) -I/opt/X11/include `pkg-config --cflags Imlib2`
	LDFLAGS:=$(LDFLAGS) `pkg-config --libs Imlib2`
else
	CFLAGS:=$(CFLAGS) `pkg-config --cflags imlib2`
	LDFLAGS:=$(LDFLAGS) `pkg-config --libs imlib2`
endif

all: $(PROG)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROG): $(MODS)
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	-rm -f $(PROG) $(MODS)
