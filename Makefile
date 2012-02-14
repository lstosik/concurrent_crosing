LDFLAGS += -lncurses
CFLAGS += -std=c99 -g -Wall -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wextra 
all: threads processes
objects/car.o:objects/car.c
objects/tram.o:objects/tram.c
objects/pedestriant.o:objects/pedestriant.c
objects.o:objects/car.o objects/tram.o objects/pedestriant.o
	ld -r objects/car.o objects/tram.o objects/pedestriant.o -o objects.o
processes: processes.c map.c terminal.c objects.o light_changer.c
threads: LDFLAGS += -lpthread
threads: CPPFLAGS += -pthread -DUSE_PTHREADS -I./
threads: threads.c config.h map.c task.h terminal.c objects.o light_changer.c

clean:
	rm -f threads
	rm -f processes
	rm -f *.o
	rm -f objects/*.o

