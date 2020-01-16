# Compiler configuration

CC= gcc
CFLAGS= \
	-Wall \
	-pedantic \
	-Werror \
	-Wshadow \
	-Wstrict-aliasing \
	-Wstrict-overflow \
	-g

# Directories

INCDIR= include
SRCDIR= src
OUTDIR= build

# Helper variables
SOURCE= $(wildcard src/*.c)

# Libraries

LIBS=-lSDL2 -lSDL2_image -lm

# Compile Objects

$(ODIR)/%.o: $(SOURCE)
	$(CC) -c -o $@ $< $(CFLAGS) `sdl2-config --cflags --libs`

# Compile Binary

$(OUTDIR)/asteroids:
	$(CC) -I$(INCDIR) $(LIBS) -I$(INCDIR) `sdl2-config --cflags --libs` $(SOURCE) -o $@

all: $(OUTDIRE)/asteroids

# Compile and run

.PHONY: run
run: $(OUTDIR)/asteroids
	./$(OUTDIR)/asteroids

# clean

.PHONY: clean
clean:
	rm $(ODIR)/*.o $(OUTDIR)/asteroids
