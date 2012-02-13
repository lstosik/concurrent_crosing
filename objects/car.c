#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "config.h"
#include "map.h"
#include "task.h"

#define LEAVE_TOP {LANE_RIGHT,MIN_Y}
#define LEAVE_BOTTOM {LANE_LEFT,MAX_Y}
#define LEAVE_RIGHT {MAX_X,LANE_BOTTOM}
#define LEAVE_LEFT {MIN_X,LANE_TOP}

#define ENTER_TOP {LANE_LEFT,MIN_Y}
#define ENTER_BOTTOM {LANE_RIGHT,MAX_Y}
#define ENTER_RIGHT {MAX_X,LANE_TOP}
#define ENTER_LEFT {MIN_X,LANE_BOTTOM}
static void try_move(task_context_t * ctx, int modX, int modY) {
    if (move_to(ctx->posX + modX, ctx->posY + modY, ctx->posX, ctx->posY)) {
        ctx->posX += modX;
        ctx->posY += modY;
        //printf("Przesuniecie do (%d,%d)\n",ctx->posX,ctx->posY);
    }
}
void* car_ep(void *arg){
	//srand(seed);
    task_context_t * ctx = arg;
	//printf("my id%d pid %d \n", ctx->id, ctx->pid);
    int from_to[4][4][2] = {
        {ENTER_LEFT, LEAVE_RIGHT, LEAVE_BOTTOM,LEAVE_TOP},
        {ENTER_RIGHT, LEAVE_LEFT, LEAVE_BOTTOM,LEAVE_TOP},
        {ENTER_TOP, LEAVE_LEFT, LEAVE_RIGHT, LEAVE_BOTTOM},
        {ENTER_BOTTOM, LEAVE_LEFT, LEAVE_RIGHT, LEAVE_TOP}
    };

	int start = rand() % 4;
	int stop = rand() % 3;
	ctx->posX = from_to[start][0][0];
	ctx->posY = from_to[start][0][1];
	while (!place_at(ctx->posX, ctx->posY)) {
		sleep(1);
	}
	 int destX = from_to[start][stop+1][0];
    int destY = from_to[start][stop+1][1];
    //printf("going from %d %d to %d %d\n", ctx->posX, ctx->posY, destX, destY);
    while (destX != ctx->posX || destY != ctx->posY) {
        if (destX > ctx->posX && ctx->posY == LANE_BOTTOM) {
            try_move(ctx, +1, 0);
        } else if (destX < ctx->posX && ctx->posY == LANE_TOP) {
            try_move(ctx, -1, 0);
        } else if (destY > ctx->posY && ctx->posX == LANE_LEFT) {
            try_move(ctx, 0, 1);
        } else if (destY < ctx->posY && ctx->posX == LANE_RIGHT) {
            try_move(ctx, 0, -1);
        }
        sleep(1);
    }
    
    remove_from(ctx->posX, ctx->posY);
	ctx->alive = 0;
	return NULL;
}
