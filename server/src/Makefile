APP=server
CC=gcc
DEPS= ../include/header.h
CFLAGS=-I ../include/
ODIR=../obj
IDIR=../include
BDIR=../bin

_DEPS=header.h
_OBJ=server.o ../obj/functions.o ../obj/database.o

OBJ=$(ODIR)/$(_OBJ)
DEPS=$(IDIR)/$(_DEPS)

server:	$(OBJ)
	$(CC) -o $(BDIR)/$(APP) $(OBJ) `mysql_config --cflags` `mysql_config --libs` -lpthread

$(ODIR)/%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# % se sustituye con el nombre del archivo
# $@ lo que esta a la izquierda del ':'
# $< el primer argumento de la derecha del ':'
# $^ toda la lista de la derecha de ':'

create:
	mkdir -p $(ODIR)/ $(BDIR)/

delete:
	rm -r $(BDIR)/ $(ODIR)/

