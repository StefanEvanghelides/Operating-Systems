CC=cc 
CFLAGS=-D_MINIX

HEADERS = array/array.h
OBJECTS = execute.o array/array.o

default: execute

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

execute: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f execute

run: execute
	./execute