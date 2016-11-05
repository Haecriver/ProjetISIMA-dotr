################################################################
#                                                              #
#  Pierre CHEVALIER - ISIMA - Projet DOTR                      #
#                                                              #
#  Fichier Makefile                                            #
#  Permet de creer les sources, les objets et l'executable     #
#  dans l'arborescence suivante :                              #
#                                                              #
#  					    $projectroot                           #
#                             |                                #
#             +---------------+----------------+               #
#             |        |           |           |               #
#           src/      obj/        res        build             #
#                                                              #
#                                                              #
################################################################

## CREATION DES VARIABLES ##

## Noms de l'arborescence ##
NAMEDIRSRC=src
NAMEDIROBJ=obj
NAMEDIREXE=build

### Compilateur ###
CC=g++

### FLAGS ###
CFLAGS=-c -ggdb -Wall $(shell pkg-config --cflags opencv)
LDFLAGS=
LIBS=$(shell pkg-config --libs opencv)

### Recuperation des sources du projet ###
SOURCES=$(wildcard $(NAMEDIRSRC)/*.cpp)

### Creation des noms des objets a creer ###
OBJECTS=$(join $(addsuffix ../$(NAMEDIROBJ)/, $(dir $(SOURCES))), $(notdir $(SOURCES:.cpp=.o)))

### Nom de l'executable ###
EXECUTABLE=$(NAMEDIREXE)/dotr

## LISTE DES REGLES ##

all:$(EXECUTABLE)
    
$(EXECUTABLE):$(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

$(OBJECTS):$(SOURCES)
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(EXECUTABLE) && rm $(OBJECTS)
	
run:
	./$(EXECUTABLE)
