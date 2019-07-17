APP=servidor
CC=gcc
CFLAGS=-I ../include/
ODIR=../obj
IDIR=../include

_DEPS=header.h 
_OBJ=servidor.o 

OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

servidor:	$(OBJ)
	$(CC) -o $(APP) $(OBJ) -pthread


$(ODIR)/%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# % se sustituye con el nombre del archivo
# $@ lo que esta a la izquierda del ':'
# $< el primer argumento de la derecha del ':'
# $^ toda la lista de la derecha de ':'

clean:
	rm -f servidor $(ODIR)/*.o

install:
	/bin/cp $(APP) /bin/

uninstall:
	/bin/rm /bin/$(APP)



