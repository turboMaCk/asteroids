# Compiler configuration

CC = gcc
CFLAGS = \
	-Wall \
	-pedantic \
	-Werror \
	-Wshadow \
	-Wstrict-aliasing \
	-Wstrict-overflow \
	-g

# Directories

ODIR = obj
INCDIR = include
SRCDIR = src
LIBDIR = lib
OUTDIR = build

# Helper variables

SOURCE = $(wildcard $(SRCDIR)/*.c) $(wildcard $(LIBDIR)/*.c)
DEPS = $(wildcard $(INCDIR)/*.h)

# Libraries

LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

# Build

all: $(OUTDIR)/asteroids

# Compile Objects

$(ODIR)/main.o: $(SOURCE) $(DEPS)
	@echo $(SOURCE)
	$(CC) -c -o $@ $< $(CFLAGS) -I$(INCDIR) `sdl2-config --cflags`

# Compile Binary

$(OUTDIR)/asteroids: $(ODIR)/main.o
	$(CC) -I$(INCDIR) $(LIBS) `sdl2-config --libs` $(SOURCE) -o $@

# Compile and run

.PHONY: run
run: $(OUTDIR)/asteroids
	./$(OUTDIR)/asteroids

# Clean

.PHONY: clean
clean:
	$(RM) $(ODIR)/*.o $(OUTDIR)/asteroids
