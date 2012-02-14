#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "map.h"
#include "task.h"

#define EP1 {SIDEWALK_RIGHT,MIN_Y}
#define EP2 {SIDEWALK_LEFT,MAX_Y}
#define EP3 {MAX_X,SIDEWALK_BOTTOM}
#define EP4 {MIN_X,SIDEWALK_TOP}

#define EP5 {SIDEWALK_LEFT,MIN_Y}
#define EP6 {SIDEWALK_RIGHT,MAX_Y}
#define EP7 {MAX_X,SIDEWALK_TOP}
#define EP8 {MIN_X,SIDEWALK_BOTTOM}
static int endpoints[8][2] = {
    EP1, EP2, EP3, EP4, EP5, EP6, EP7, EP8,
};
static void try_move(task_context_t * ctx, int modX, int modY) {
    if (shr_move_to(ctx->posX + modX, ctx->posY + modY, ctx->posX, ctx->posY)) {
        ctx->posX += modX;
        ctx->posY += modY;
    }
}


void flood_fill(unsigned int * map_p, int x, int y,unsigned int c) {
    if (x < MIN_X || x > MAX_X) {
        return;
    }
    if (y < MIN_Y || y > MAX_Y) {
        return;
    }
    if (y != SIDEWALK_BOTTOM && y != SIDEWALK_TOP && x != SIDEWALK_LEFT && x != SIDEWALK_RIGHT) {
        return;
    }
    unsigned int real_x = x +(-MIN_X);
    unsigned int real_y = y +(-MIN_Y);
    if (map_p[real_x * TOTAL_Y + real_y] > c) {
        map_p[real_x * TOTAL_Y + real_y] = c;
        flood_fill(map_p, x + 1, y, c + 1);
        flood_fill(map_p, x - 1, y, c + 1);
        flood_fill(map_p, x, y + 1, c + 1);
        flood_fill(map_p, x, y - 1, c + 1);
    }
}

unsigned int get_value(unsigned int * map_p, int x, int y) {
    if (x < MIN_X || x > MAX_X) {
        return 0xFFFFFFFF;
    }
    if (y < MIN_Y || y > MAX_Y) {
        return 0xFFFFFFFF;
    }
    unsigned int real_x = x +(-MIN_X);
    unsigned int real_y = y +(-MIN_Y);
    return map_p[real_x * TOTAL_Y + real_y];
}



void* pedestriant_ep(void *arg) {
    task_context_t * ctx = arg;
    int start, stop;
    do {
        start = rand() % 8;
        stop = rand() % 8;
    } while(start == stop);
	ctx->posX = endpoints[start][0];
	ctx->posY = endpoints[start][1];
	while (!shr_place_at(ctx->posX, ctx->posY)) {
		sleep(1);
	}
    int destX = endpoints[stop][0];
    int destY = endpoints[stop][1];
    //printf("going from %d %d to %d %d\n", ctx->posX, ctx->posY, destX, destY);
    while (destX != ctx->posX || destY != ctx->posY) {
        unsigned int * map_p = malloc(TOTAL_X*TOTAL_Y* sizeof (int));
        memset(map_p, 0xFF, TOTAL_X*TOTAL_Y* sizeof (int));
        flood_fill(map_p, destX, destY, 0);
        unsigned int c = get_value(map_p, ctx->posX+1, ctx->posY);
        int dx = +1;
        int dy =0;
        if (c> get_value(map_p, ctx->posX-1, ctx->posY)) {
            c = get_value(map_p, ctx->posX-1, ctx->posY);
            dx = -1;
            dy =0;
        }
        if (c> get_value(map_p, ctx->posX, ctx->posY+1)) {
            c = get_value(map_p, ctx->posX, ctx->posY+1);
            dx = 0;
            dy = 1;
        }
        if (c> get_value(map_p, ctx->posX, ctx->posY-1)) {
            c = get_value(map_p, ctx->posX, ctx->posY-1);
            dx = 0;
            dy = -1;
        }
        free(map_p);
        
        try_move(ctx,dx,dy);
        sleep(1);
    }
    
    shr_remove_from(ctx->posX, ctx->posY);
	ctx->alive = 0;
	return NULL;
}
