LDFLAGS += -lncurses
CFLAGS += -std=c99 -g
all: process threads
process.o: process.c

process: process.o

threads: LDFLAGS += -lpthread
threads: CPPFLAGS += -pthread
threads: threads.c

clean:
	rm -f threads
	rm -f process
	rm -f *.o

