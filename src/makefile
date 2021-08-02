CC=gcc
CFLAGS=-I. 
OBJ = keyes.o 
LDFLAGS = -Wl,-rpath -Wl,/usr/local/lib  -lwiringPi -llirc_client 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

keyes: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	
.PHONY: clean

clean:
	rm -f *.o  

.PHONY: indent
indent:
	indent -kr -bl -bli0 -ci2 -cli2 -i2 -l80 -nut *.c 
	
.PHONY: install
install:
	cp -f -v keyes /home/pi/bin/
 
  
