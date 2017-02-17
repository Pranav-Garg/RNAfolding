CC = gcc
CPP = g++
DEBUG = -g
CFLAGS = -Wall $(DEBUG)

all: fold.exe

foldingc.o: foldingc.c foldingc.h
	$(CC) -c $(CFLAGS) -o $@ $<

foldingcpp.o: foldingcpp.cpp foldingcpp.h
	$(CPP) -c $(CFLAGS) -o $@ $<

fold.o: fold.cpp foldingc.h foldingcpp.h
	$(CPP) -c $(CFLAGS) -o $@ $<

fold.exe: foldingc.o foldingcpp.o fold.o
	$(CPP) $(CFLAGS) -o $@ $+

clean:
	$(RM) *~ *.o *.exe a.out
