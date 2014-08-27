
SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)

EXE = foo

CPPFLAGS =

CC = tau_cc.sh
CFLAGS = -g

LD = $(CC)
LDFLAGS =
LIBS =



.PHONY: all clean tests

all: $(EXE)

clean:
	@rm -fv $(EXE) $(OBJ)

$(EXE): $(OBJ)
	$(LD) -o $(EXE) $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.c $(HDR) Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

