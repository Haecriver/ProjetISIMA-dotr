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

SRCCALDIR = calibration

### Compilateur ###
CC = g++

### FLAGS ###
CFLAGS = -c -g -std=c++11 -ggdb -Wall $(shell pkg-config --cflags opencv)
LDFLAGS =
LIBS = $(shell pkg-config --libs opencv)

### Liste des dependances de compilations (.hpp) ###
DEPS =

### Nom des executables ###
_EXEC = dotr stats line calibration imagelist_creator cross_ratio soutenance
EXEC = $(patsubst %,$(EXEDIR)/%,$(_EXEC))

### Fichiers communs a tous les executables ###
_OBJ_COM = window display composante filtre bthreshold dilatation \
			erosion grayscale etiquetage adathreshold detectShape \
			detectedBlob linePoint line detectLine axe poseEstimation
			
OBJ_COM = $(patsubst %,$(OBJDIR)/%.o,$(_OBJ_COM))

### Fichiers dedies a un executable ###
#### dotr ####
_OBJ_VID = main_video
OBJ_VID = $(patsubst %,$(OBJDIR)/%.o,$(_OBJ_VID))

_OBJ_STA = main_stats
OBJ_STA = $(patsubst %,$(OBJDIR)/%.o,$(_OBJ_STA))

_OBJ_LINE = main_line
OBJ_LINE = $(patsubst %,$(OBJDIR)/%.o,$(_OBJ_LINE))

_OBJ_SOUT = main_expose
OBJ_SOUT = $(patsubst %,$(OBJDIR)/%.o,$(_OBJ_SOUT))

### Listes des fichiers objets ###
OBJ = $(OBJ_COM) $(OBJ_VID) $(OBJ_STA) $(OBJ_LINE) $(OBJ_SOUT) 

## LISTE DES REGLES ##

all:$(EXEC)

#$(EXEC):$(OBJ)
#	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

build/dotr:$(OBJ_VID) $(OBJ_COM)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

build/stats:$(OBJ_STA) $(OBJ_COM)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@
	
build/line:$(OBJ_LINE) $(OBJ_COM)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

build/soutenance:$(OBJ_SOUT) $(OBJ_COM)
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(OBJDIR)/%.o:$(SRCDIR)/%.cpp $(DEPS)
	$(CC) -o $@ $< $(CFLAGS)
	
# Projet calibration
build/calibration:$(OBJDIR)/calibration.o
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@
	
build/imagelist_creator:$(OBJDIR)/imagelist_creator.o
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@
	
$(OBJDIR)/%.o: $(SRCCALDIR)/%.cpp
	$(CC) -o $@ $< $(CFLAGS)
	
# Cross ratio
build/cross_ratio:$(OBJDIR)/crossRatio.o
	$(CC) $^ -o $@
	
clean:
	rm $(EXEC); rm $(OBJ)
	
run:
	./$(EXEC)
