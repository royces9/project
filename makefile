LDFLAGS = -lm -lreadline -flto
FLAGS = -MMD -Wall
DEBUG_FLAGS =
CC = gcc

CFILES = $(wildcard *.c)

USR = /usr/local/bin/calc
TARGET = calc2

all: DEBUG_FLAGS += -g
all: calc

usr: TARGET = $(USR)
usr: FLAGS += -O2
usr: calc

calc: $(CFILES:.c=.o)
	$(CC) $^ -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(FLAGS) $(DEBUG_FLAGS) -c $<

.PHONY: clean
clean:
	del *.o *.d vgcore.*

.PHONY: cleano
cleano:
	del *.o

.PHONY: cleand
cleand:
	del *.d

-include $(CFILES:.c=.d)
