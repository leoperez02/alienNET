#archivo makefile

# Variables generales para el makefile
PROYECTO=bin/alien_server
DESTDIR=/usr/local/bin
CFLAGS=-Wall -pedantic 
LDFLAGS=-lm -lpthread 
CC=gcc
SRCDIR=src
BUILDDIR=build
INC=-I include

all: $(PROYECTO)

# Reduccion 1
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CFLAGS) $(INC)

# Remplaza a:
#procesamiento.o: procesamiento.c
#	$(CC) -c procesamiento.c $(CFLAGS)

#archivos.o: archivos.c
#	$(CC) -c archivos.c $(CFLAGS)

#muestreo.o: muestreo.c
#	$(CC) -c muestreo.c $(CFLAGS)

# TAREA
# Uso de wildcards

# Anterior
#$(PROYECTO): procesamiento.o archivos.o muestreo.o

# Simplificacion con wildcars y sustitucion de patrones:
# Lista de todos los archivos fuente .c
s_files = $(wildcard $(SRCDIR)/*.c)
# Sustitucion de *.c por *.o en la lista de archivos fuente
$(PROYECTO): $(s_files:$(SRCDIR)/*.c=$(BUILDDIR)/*.o)
	$(CC) $^ -o $@ $(LDFLAGS) $(INC) 

# Variables especiales $^ y $@
#	$(CC) $^ -o $(PROYECTO) $(LDFLAGS) 
#	$(CC) procesamiento.o archivos.o muestreo.o -o $(PROYECTO) $(LDFLAGS) 

install: all
	if [ ! -d $(DESTDIR) ] ; then \
		sudo mkdir $(DESTDIR); \
	fi; \
	sudo cp $(PROYECTO) $(DESTDIR)

uninstall:
	sudo rm $(DESTDIR)/$(PROYECTO)

.PHONY: clean

clean:
	rm -r $(BUILDDIR) 
	rm $(PROYECTO)
	mkdir $(BUILDDIR)
	
# Clean region 4
.PHONY: barrele

barrele: clean
