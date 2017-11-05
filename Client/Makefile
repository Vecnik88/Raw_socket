CC = gcc
CFLAGS = -Wall -ansi -pedantic -g3 -O0
LDFLAGS =
SOURCES = ./src/raw_client.c  ./src/print.c
OBJECTS = $(SOURCES:.c=.o)
INSTPATH = /usr/bin
EXECUTABLE = Client

.PHONY: all clean run install uninstall

all: clean $(SOURCES) $(EXECUTABLE)
	rm -f ./src/*.o
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

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
