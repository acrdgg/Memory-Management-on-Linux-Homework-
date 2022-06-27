
CFLAGS = -g -fsanitize=address
LDFLAGS = -I.
DEPS = mymalloc.h
SOURCES= mymalloc.c memperf.c 
OBJECTS= mymalloc.o memperf.o
EXECUTABLE=memperf

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(LDFLAGS)

memperf: $(OBJECTS)
	gcc $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o *~ mymalloc
