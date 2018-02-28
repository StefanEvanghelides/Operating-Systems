CC=cc 
CFLAGS=-D_MINIX

HEADERS = 
OBJECTS = duplicates.o

default: duplicates

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

duplicates: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f duplicates

run: duplicates
	./duplicates
