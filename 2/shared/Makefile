CC=cc 
CFLAGS=-D_MINIX

HEADERS = 
OBJECTS = pingpong.o

default: pingpong

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

pingpong: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)
	-rm -f $(OBJECTS)

clean:
	-rm -f $(OBJECTS)
	-rm -f pingpong

run: shell
	./pingpong
