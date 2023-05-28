# Makefile
EXEC=mingw-bundledlls

ifeq ($(OS),Windows_NT)
	CC=gcc
	LDFLAGS=-fno-exceptions
	EXEC:=$(EXEC).exe
else
	CC=gcc
	LDFLAGS=-fno-exceptions
endif

CFLAGS=-c -Wall -O3
STRIP_EXE=strip

OBJ=mingw-bundledlls.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(STRIP_EXE) $(EXEC)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	-$(RM) *.o
	-$(RM) $(EXEC)