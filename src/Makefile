CFLAGS= -O3 -s -mwindows -Wall -fomit-frame-pointer
SLIBS= -lcomctl32
OBJECTS= minefield.o timer.o minimine.o guires.o
CC= g++
PROGRAM= minimine
RESC= res/minimine.ico res/window.bmp res/elements.bmp

$(PROGRAM): $(OBJECTS)
	$(CC) -o$(PROGRAM) $(OBJECTS) $(SLIBS) $(CFLAGS)

minefield.o: minefield.cpp minefield.h
	$(CC) $(CFLAGS) minefield.cpp -c

timer.o: timer.cpp timer.h
	$(CC) $(CFLAGS) timer.cpp -c

minimine.o: minimine.cpp
	$(CC) $(CFLAGS) minimine.cpp -c

guires.o: minimine.rc $(RESC)
	windres minimine.rc -o guires.o