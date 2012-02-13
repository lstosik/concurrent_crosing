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
task_context_t *cars;
task_context_t *my_ctx = NULL;
int seed;
void car(void);
key_t   mykey;
int semid;
int *map;
int * map_alloc(void);
task_context_t * cars_alloc(void);
int main(int argc, char **argv) {
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
	pid_t childpid;
	seed = time(NULL);
	for (int i = 0; i < MAX_CARS; i++) {
		my_ctx = &cars[i];
		childpid = fork();
		if (childpid == 0) {
			car();
			return EXIT_SUCCESS;
		} else {
			cars[i].pid = childpid;
		}
		seed++;

	}
	  //loop_graphic();
	loop();
	return (EXIT_SUCCESS);
}
#define LEAVE_TOP {1,MIN_Y}
#define LEAVE_BOTTOM {-1,MAX_Y}
#define LEAVE_RIGHT {MAX_X,1}
#define LEAVE_LEFT {MIN_X,-1}
void carTryMove(task_context_t * ctx, int modX, int modY) {
    if (move_to(ctx->id,ctx->posX + modX, ctx->posY + modY, ctx->posX, ctx->posY)) {
        ctx->posX += modX;
        ctx->posY += modY;
        //printf("Przesuniecie do (%d,%d)\n",ctx->posX,ctx->posY);
    }
}
void car() {
	srand(seed);
	printf("my id%d pid %d \n", my_ctx->id, my_ctx->pid);
	int from_to[4][4][2] = {
		{
			{MIN_X, 1}, LEAVE_RIGHT, LEAVE_BOTTOM, LEAVE_TOP},
		{
			{MAX_X, -1}, LEAVE_LEFT, LEAVE_BOTTOM, LEAVE_TOP},
		{
			{-1, MIN_Y}, LEAVE_LEFT, LEAVE_RIGHT, LEAVE_BOTTOM},
		{
			{1, MAX_Y}, LEAVE_LEFT, LEAVE_RIGHT, LEAVE_TOP}
	};

	int start = rand() % 4;
	int stop = rand() % 3;
	my_ctx->posX = from_to[start][0][0];
	my_ctx->posY = from_to[start][0][1];
	while (!place_at(my_ctx->id, my_ctx->posX, my_ctx->posY)) {
		sleep(1);
	}
	 int destX = from_to[start][stop+1][0];
    int destY = from_to[start][stop+1][1];
    //printf("going from %d %d to %d %d\n", my_ctx->posX, my_ctx->posY, destX, destY);
    while (destX != my_ctx->posX || destY != my_ctx->posY) {
        if (destX > my_ctx->posX && my_ctx->posY == 1) {
            carTryMove(my_ctx, +1, 0);
        } else if (destX < my_ctx->posX && my_ctx->posY == -1) {
            carTryMove(my_ctx, -1, 0);
        } else if (destY > my_ctx->posY && my_ctx->posX == -1) {
            carTryMove(my_ctx, 0, 1);
        } else if (destY < my_ctx->posY && my_ctx->posX == 1) {
            carTryMove(my_ctx, 0, -1);
        }
        sleep(1);
    }
    
    remove_from(my_ctx->id, my_ctx->posX, my_ctx->posY);
	my_ctx->alive = 0;
	return;
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
	pid_t childpid;

	my_ctx = &cars[i];
	childpid = fork();
	if (childpid == 0) {
		car();
		exit(EXIT_SUCCESS);
	} else {
		cars[i].pid = childpid;
	}
	seed++;



}

