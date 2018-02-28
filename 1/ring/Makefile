CC=cc 
CFLAGS=-D_MINIX

HEADERS = 
OBJECTS = ring.o

default: ring

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

ring: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f ring

run: ring
	./ring
