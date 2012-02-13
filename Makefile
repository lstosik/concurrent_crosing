LDFLAGS += -lncurses
CFLAGS += -std=c99 -g -Wall -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wextra 
all: threads processes
processes: processes.c map.c terminal.c

threads: LDFLAGS += -lpthread
threads: CPPFLAGS += -pthread -DUSE_PTHREADS
threads: threads.c config.h map.c task.h terminal.c

clean:
	rm -f threads
	rm -f processes
	rm -f *.o

