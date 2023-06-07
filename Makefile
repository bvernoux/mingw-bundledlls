# Makefile
EXEC=mingw-bundledlls

ifeq ($(OS),Windows_NT)
	EXEC:=$(EXEC).exe
endif

CC=gcc
LDFLAGS=-fno-exceptions -s

CFLAGS=-c -Wall -O3

OBJ=mingw-bundledlls.o Find_DLL_Dependencies.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	-$(RM) *.o
	-$(RM) $(EXEC)