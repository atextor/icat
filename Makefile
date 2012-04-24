GCC=gcc
CCFLAGS=-Wall -pedantic -std=c99
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



