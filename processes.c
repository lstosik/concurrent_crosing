#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#include "task.h"
#include "config.h"
#include "map.h"
#include "terminal.h"
#include "objects/car.h"
#include "objects/tram.h"
#include "objects/pedestriant.h"
task_context_t *cars;
task_context_t *my_ctx = NULL;
int seed;

key_t   mykey;
int semid;
int *map;
int * map_alloc(void);
task_context_t * cars_alloc(void);
int main(int  __attribute__((unused))argc,  __attribute__((unused)) char **argv) {
	char buf[50];
	char * dirname = get_current_dir_name();
	snprintf(buf, 50, "%s/%s",dirname,argv[0]);
	free(dirname);
	mykey = ftok(buf,0);
	
	semid = semget( mykey, 1, IPC_CREAT | 0660 );
	map_unlock();
	map = map_alloc();
	cars = cars_alloc();
	printf("map address %p\n",(void*)map);
	for (int i = 0; i < MAX_CARS; i++) {
		cars[i].id = 1 + i;
		cars[i].alive = 1;
	}
	seed = time(NULL);
	for (int i = 0; i < MAX_CARS; i++) {
        task_start(i);
		seed++;

	}
	  //loop_graphic();
	loop();
	return (EXIT_SUCCESS);
}


int open_segment(key_t keyval, int segsize) {
	int shmid;

	if ((shmid = shmget(keyval, segsize, IPC_CREAT | 0660)) == -1) {
		return (-1);
	}

	return (shmid);
}

char *attach_segment(int shmid) {
	return (shmat(shmid, 0, 0));
}
int * map_alloc(void) {
	int segment = open_segment(mykey, sizeof(TOTAL_X*TOTAL_Y*sizeof(int)));
	return (int*)attach_segment(segment);
}

task_context_t * cars_alloc(void) {
	int segment = open_segment(mykey, sizeof(MAX_CARS*sizeof(task_context_t)));
	return (task_context_t*)attach_segment(segment);
}


struct sembuf sem_lock = { 0, -1, 0 };
struct sembuf sem_unlock = { 0, 1, 0 };//IPC_NOWAIT
void map_lock(void) {
	semop(semid, &sem_lock, 1);
}

void map_unlock(void) {
semop(semid, &sem_unlock, 1);
}

void task_start(int i) {
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
    my_ctx = &cars[i];
	pid_t childpid = fork();
	if (childpid == 0) {
        srand(seed);
		entry_point(&cars[i]);
		exit(EXIT_SUCCESS);
	} else {
		cars[i].pid = childpid;
	}
	seed++;



}

