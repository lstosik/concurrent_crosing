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

task_context_t *cars;
void * car(void *arg);
int *map;
int curses = 1;
int main(int argc, char **argv) {
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
#define LEAVE_TOP {1,MIN_Y}
#define LEAVE_BOTTOM {-1,MAX_Y}
#define LEAVE_RIGHT {MAX_X,1}
#define LEAVE_LEFT {MIN_X,-1}

void carTryMove(task_context_t * ctx, int modX, int modY) {
	if (move_to(ctx->id, ctx->posX + modX, ctx->posY + modY, ctx->posX, ctx->posY)) {
		ctx->posX += modX;
		ctx->posY += modY;
		if (!curses) {
			printf("[%d]Przesuniecie do (%d,%d)\n", ctx->id, ctx->posX, ctx->posY);
		}
	} else {
		if (!curses) {
			printf("[%d]Nie udalo sie ruszyc do (%d,%d)\n", ctx->id, ctx->posX, ctx->posY);
		}
	}

}
void* car(void *arg) {
    task_context_t * ctx = arg;
    int from_to[4][4][2] = {
        {{MIN_X,1}, LEAVE_RIGHT, LEAVE_BOTTOM,LEAVE_TOP},
        {{MAX_X,-1}, LEAVE_LEFT, LEAVE_BOTTOM,LEAVE_TOP},
        {{-1,MIN_Y}, LEAVE_LEFT, LEAVE_RIGHT, LEAVE_BOTTOM},
        {{1,MAX_Y}, LEAVE_LEFT, LEAVE_RIGHT, LEAVE_TOP}
    };
    
    int start = rand() % 4;
    int stop = rand() % 3;
    ctx->posX = from_to[start][0][0];
    ctx->posY = from_to[start][0][1];
    while(!place_at(ctx->id, ctx->posX, ctx->posY)) {
        sleep(1);
    }
    int destX = from_to[start][stop+1][0];
    int destY = from_to[start][stop+1][1];
	if (!curses) {
    printf("[%d]going from %d %d to %d %d\n", ctx->id, ctx->posX, ctx->posY, destX, destY);
	}
    while (destX != ctx->posX || destY != ctx->posY) {
        if (destX > ctx->posX && ctx->posY == 1) {
            carTryMove(ctx, +1, 0);
        } else if (destX < ctx->posX && ctx->posY == -1) {
            carTryMove(ctx, -1, 0);
        } else if (destY > ctx->posY && ctx->posX == -1) {
            carTryMove(ctx, 0, 1);
        } else if (destY < ctx->posY && ctx->posX == 1) {
            carTryMove(ctx, 0, -1);
        }
        sleep(1);
    }
    
    remove_from(ctx->id, ctx->posX, ctx->posY);
	if (!curses) {
    printf("endof %d\n", ctx->id);
	}
	ctx->alive = 0;
	
	return NULL;
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
	pthread_create(&cars[i].thread, NULL, &car, &cars[i]);
}
