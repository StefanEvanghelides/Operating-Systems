CC=cc 
CFLAGS=-D_MINIX

HEADERS = parser/parser.h list/list.h
OBJECTS = shell.o parser/parser.o list/list.o

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