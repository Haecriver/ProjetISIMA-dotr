CC=g++
CFLAGS=-c -ggdb -Wall $(shell pkg-config --cflags opencv)
LDFLAGS=
LIBS=$(shell pkg-config --libs opencv)
SOURCES=src/main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=build/dotr

all:$(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE):$(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(EXECUTABLE); rm $(OBJECTS)
	
run:
	./$(EXECUTABLE)
