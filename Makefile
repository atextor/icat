GCC=gcc
CCFLAGS=-Wall -pedantic -std=c99 -D_BSD_SOURCE
LDFLAGS=-lImlib2
PROG=icat
MODS=icat.o

all: $(PROG)

%.o: %.c
	$(GCC) -c $(CCFLAGS) -o $@ $<

$(PROG): $(MODS)
	$(GCC) -o $@ $< $(LDFLAGS)

clean:
	-rm -f $(PROG) $(MODS)



