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
SRCDIR = src
OBJDIR = obj
EXEDIR = build

### Compilateur ###
CC = g++

### FLAGS ###
CFLAGS = -c -std=c++11 -ggdb -Wall $(shell pkg-config --cflags opencv)
LDFLAGS =
LIBS = $(shell pkg-config --libs opencv)

### Creation des noms des objets a creer ###
_OBJ = main.o composante.o 
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

### Nom de l'executable ###
_EXEC = dotr
EXEC = $(EXEDIR)/$(_EXEC)

## LISTE DES REGLES ##

all:$(EXEC)
    
$(EXEC):$(OBJ) 
	$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm $(EXEC); rm $(OBJ)
	
run:
	./$(EXEC)