#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <curses.h>
#include <sys/types.h>
#ifndef USE_PTHREADS
#define USE_PTHREADS
#endif
#include "task.h"
#include "config.h"
#include "map.h"
#include "terminal.h"
#include "objects/car.h"
#include "objects/tram.h"
#include "objects/pedestriant.h"

task_context_t *cars;

int *map;
int curses = 1;
int main(int argc, __attribute__((unused)) char **argv) {
	if (argc > 1) {
		curses = 0;
	}
	void * map_p = calloc(TOTAL_X*TOTAL_Y, sizeof (int));
	map = (int*) map_p;
	cars = calloc(MAX_CARS, sizeof (task_context_t));
	for (int i = 0; i < MAX_CARS; i++) {
		task_start(i);
	}
	if (!curses) {
		loop();
	} else {
		loop_graphic();
	}

	return 0;
}

pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER; 
void map_lock(void) {
	pthread_mutex_lock(&map_mutex);
}

void map_unlock(void) {
	pthread_mutex_unlock(&map_mutex);
}

void task_start(int i) {
	if (cars[i].id != 0) {
		pthread_join(cars[i].thread, NULL);
	}
	cars[i].id = 1 + i;
	cars[i].alive = 1;
    void *(*entry_point) (void *);
    switch(rand() %6) {
        case 0:
        case 1:
        case 2:
            cars[i].type = car;
            entry_point = &car_ep;
            break;
        case 3:
        case 4:
            cars[i].type = pedestriant;
            entry_point = &pedestriant_ep;
            break;
        default:
            cars[i].type = tram;
            entry_point = &tram_ep;
    }
    
    pthread_create(&cars[i].thread, NULL, entry_point, &cars[i]);
}
