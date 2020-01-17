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

ODIR= obj
INCDIR= include
SRCDIR= src
OUTDIR= build

# Helper variables
SOURCE= $(wildcard $(SRCDIR)/*.c)

# Libraries

LIBS=-lSDL2 -lSDL2_image -lm

# Compile Objects

$(ODIR)/%.o: $(SOURCE)
	@echo $(SOURCE)
	$(CC) -c -o $@ $< $(CFLAGS) -I$(INCDIR) `sdl2-config --cflags`

# Compile Binary

$(OUTDIR)/asteroids: $(ODIR)/%.o
	$(CC) -I$(INCDIR) $(LIBS) `sdl2-config --libs` $(SOURCE) -o $@

all: $(OUTDIRE)/asteroids

# Compile and run

.PHONY: run
run: $(OUTDIR)/asteroids
	./$(OUTDIR)/asteroids

# clean

.PHONY: clean
clean:
	$(RM) $(ODIR)/*.o $(OUTDIR)/asteroids
