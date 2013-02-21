GCC=gcc
CCFLAGS=-Wall -pedantic -std=c99 -D_BSD_SOURCE
LDFLAGS=-lImlib2
PROG=icat
MODS=icat.o

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo unknown')

ifeq ($(uname_S),Darwin)
	CCFLAGS:=$(CCFLAGS) -I/opt/X11/include
endif

all: $(PROG)

%.o: %.c
	$(GCC) -c $(CCFLAGS) -o $@ $<

$(PROG): $(MODS)
	$(GCC) -o $@ $< $(LDFLAGS)

clean:
	-rm -f $(PROG) $(MODS)



