FORMAT=.cpp
CC=g++
SRC=$(wildcard *$(FORMAT))
CLASSES=$(wildcard Classes/*$(FORMAT))
LIBS=$(wildcard Libs/*$(FORMAT))
SOIL=$(wildcard Libs/soil/*c)
EXEC=Bin/TCG
CFLAGS=-w -Wall -std=c++11
LDFLAGS=-lglut -lGLU -lGL -lm -lopenal -lGLEW -lSOIL
LDFLAGSMAC=-lm -framework OpenGL -framework OpenAL -framework GLUT -framework CoreFoundation #-F/usr/lib/ -framework libGLEW.2.0.0
clean:
	rm -rf *.o $(EXEC)

all: make run

make:
	$(CC) $(SRC) $(CLASSES) $(LIBS) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

makemac:
	$(CC) $(SRC) $(CLASSES) $(LIBS) $(SOIL) -o $(EXEC)MacOSX $(CFLAGS) $(LDFLAGSMAC)

runmac: makemac
	./$(EXEC)MacOSX

run: make
	./$(EXEC)
