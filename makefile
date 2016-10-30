CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=src/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=build/dotr

all:$(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE):$(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
