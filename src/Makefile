CC= g++
CFLAGS= -O3 -s -mwindows -Wall -fomit-frame-pointer
LDFLAGS= -static-libgcc
OBJECTS= minefield.o timer.o minimine.o guires.o
RESC= res/minimine.ico res/window.bmp res/elements.bmp

minimine: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

minefield.o: minefield.cpp minefield.h
	$(CC) $(CFLAGS) -c $<

timer.o: timer.cpp timer.h
	$(CC) $(CFLAGS) -c $<

minimine.o: minimine.cpp
	$(CC) $(CFLAGS) -c $<

guires.o: minimine.rc $(RESC)
	windres minimine.rc -o $@

clean:
	rm -f core *.o *.exe
