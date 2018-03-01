CC=cc 
CFLAGS=-D_MINIX

HEADERS = array/array.h parser/parser.h
OBJECTS = shell.o array/array.o parser/parser.o

default: shell

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

shell: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f shell

run: shell
	./shell