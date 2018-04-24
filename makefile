CC = gcc
FLAGS = -lm -lreadline -I. -flto -Wall -O2
HEADERS = $(wildcard *.h)
CFILES = $(wildcard *.c)
OFILES = $(wildcard *.o)

calc2: $(OFILES)
	$(CC) $(OFILES) -o calc2 $(FLAGS)

debug: $(OFILES)
	$(CC) -g $(OFILES) -o calc2 $(FLAGS)

matrix.o: matrix.c matrix.h
	%(CC) -c matrix.c

stack.o: stack.c stack.h
	$(CC) -c stack.c

multi.o: multi.c multi.h stack.h sya.h
	$(CC) -c multi.c

onearg.o: onearg.c onearg.h multi.h stack.h file.h operator.h
	$(CC) -c onearg.c

sya.o: sya.c sya.h stack.h onearg.h operator.h
	$(CC) -c sya.c

calc.o: calc.c stack.h sya.h
	$(CC) -c calc.c

file.o: file.c file.h stack.h sya.h fileStruct.h
	$(CC) -c file.c

operator.o: operator.c operator.h
	$(CC) -c operator.c

fileStruct.o: fileStruct.c fileStruct.h
	$(CC) -c fileStruct.c

usr: $(OFILES)
	$(CC) $(OFILES) -o /usr/local/bin/calc $(FLAGS)

del: $(OFILES)
	del $(OFILES)

