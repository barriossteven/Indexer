CC = gcc
CCFLAGS = -g -Wall

all: index

index: indexer.o idk.o
	$(CC) $(CCFLAGS) indexer.o idk.o -o index
indexer.o: indexer.c indexer.h idk.h
	$(CC) $(CCFLAGS) -c indexer.c indexer.h idk.h
idk.o: idk.c idk.h
	$(CC) $(CCFLAGS) -c idk.c idk.h
clean:
	rm -f index
	rm -f *.o
	rm -f *.a
	rm -f *.gch
