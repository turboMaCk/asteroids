INCDIR=include
CC=gcc
CFLAGS=-I$(INCDIR) -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow

ODIR=obj
SRCDIR=src
OUTDIR=build

LIBS=-lSDL2 -lSDL2_image

_DEPS=
DEPS=$(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ=main.o

OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) `sdl2-config --cflags --libs` -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: run
run: main
	$(OUTDIR)/main

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(SRCDIR)*~ core $(INCDIR)/*~
