CC = gcc
CFLAGS = -Wall -ansi -pedantic -g3 -O0
LDFLAGS = -pthread
SOURCES = ./src/server.c ./src/thread_func.c
OBJECTS = $(SOURCES:.c=.o)
INSTPATH = /usr/bin
EXECUTABLE = Server

.PHONY: all clean run install uninstall

all: clean $(SOURCES) $(EXECUTABLE)
	rm -f ./src/*.o
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE)

install:
	sudo install $(EXECUTABLE) $(INSTPATH)

uninstall:
	sudo rm $(INSTPATH)/$(EXECUTABLE)

run:
	$(INSTPATH)/$(EXECUTABLE)
