CC = GCC

CFLAGS = -Wall -Wextra

T1 = optt
T2 = opts

$(T1) : optt.o
  $(CC) $(CFLAGS) -o $@ $^

$(T2) : opts.o
  $(CC) $(CFLAGS) -o $@ $^

optt.o : optt.c optt.h
  $(CC) $(CFLAGS) -c -o $@ $<

opts.o : opts.c opts.h
  $(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f optt.o opts.o $(T1) $(T2)
  
