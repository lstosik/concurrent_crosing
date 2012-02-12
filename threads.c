#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <curses.h>
#include <sys/types.h>

typedef struct {
    int id;
    pthread_t thread;
    int posX;
    int posY;
    int alive;
} thread_context_t;
#define MIN_X -10
#define MIN_Y -10
#define MAX_X 10
#define MAX_Y 10
#define MAX_CARS 4
volatile int map[-(MIN_X-MAX_X-1)][-(MIN_Y-MAX_Y-1)];
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER; 
thread_context_t cars[MAX_CARS];
void * car(void *arg);
int place_at(int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    pthread_mutex_lock(&map_mutex);
    if (map[realX][realY] == 0) {
        map[realX][realY] = 1;
        pthread_mutex_unlock(&map_mutex);
        return 1;
    } else {
        pthread_mutex_unlock(&map_mutex);
        return 0;
    }
}

int remove_from(int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    pthread_mutex_lock(&map_mutex);
    map[realX][realY] = 0;
    pthread_mutex_unlock(&map_mutex);
    return 1;
}

int move_to(thread_context_t * ctx, int x, int y, int fromX, int fromY) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    unsigned int realFromX = fromX+(-MIN_X);
    unsigned int realFromY = fromY+(-MIN_Y);
    pthread_mutex_lock(&map_mutex);
    if (!map[realFromX][realFromY]) {
        //fprintf(stderr,"blad przy ruszaniu z %d, %d do %d,%d, pole zrodlowe nie jest zajete\n",fromX,fromY,x,y);
        //exit(0);
    };
    if (map[realX][realY] == 0) {
        map[realX][realY] = ctx->id;
        map[realFromX][realFromY] = 0;
        pthread_mutex_unlock(&map_mutex);
        return 1;
    } else {
        pthread_mutex_unlock(&map_mutex);
        return 0;
    }
}
int main() {
    for (int i=0;i<MAX_CARS;i++) {
        cars[i].id = 1 + i;
    }
    for (int i=0;i<MAX_CARS;i++) {
        cars[i].alive = 1;
        pthread_create(&cars[i].thread, NULL, &car, &cars[i]);
    }
    intrflush(stdscr,FALSE);

int x,y,Pid,i;

//
initscr();                        
getmaxyx(stdscr,y,x);

if ((y<20) || (x<60)) {
    endwin();               
    return 1;
}

start_color();
init_pair(1,COLOR_GREEN,COLOR_GREEN);
init_pair(2,COLOR_GREEN,COLOR_BLACK);
init_pair(3,COLOR_RED,COLOR_BLACK);
init_pair(4,COLOR_BLACK,COLOR_BLACK);
init_pair(5,COLOR_WHITE,COLOR_BLACK);
init_pair(6,COLOR_BLACK,COLOR_YELLOW);
init_pair(7,COLOR_MAGENTA,COLOR_WHITE);
init_pair(8,COLOR_BLACK,COLOR_CYAN);
init_pair(9,COLOR_WHITE,COLOR_BLACK);
init_pair(10,COLOR_BLACK,COLOR_RED);
init_pair(11,COLOR_RED,COLOR_MAGENTA);
init_pair(12,COLOR_WHITE,COLOR_WHITE);
nodelay(stdscr, 1);
cbreak();
while(getch() == ERR) {
    clear();
    pthread_mutex_lock(&map_mutex);
    attron(COLOR_PAIR(2));
    for (int tx=MIN_X;tx<MAX_X;tx++) {
        for (int ty=MIN_Y;ty<MAX_Y;ty++) {
            int realX = tx+(-MIN_X);
            int realY = ty+(-MIN_Y);
            mvprintw(realY,2*realX,"%02d",map[realX][realY]);
        }
    }
     attron(COLOR_PAIR(5));
/*poziomo linie*/
    for (int j = 0; j < (-MIN_X); j++) {
        if (j % 2 == 0) {
            mvprintw((-MIN_Y), j, "-");
        } else {
            mvprintw((-MIN_Y), j, " ");
        }
    }
     attron(COLOR_PAIR(6));
    for(int k=0;k<MAX_CARS;k++) {
        if (cars[k].alive) {
            mvprintw((cars[k].posY+(-MIN_Y)),2*(cars[k].posX+(-MIN_X)),"%02d",k);
        } else {
            cars[k].alive = 1;
            pthread_create(&cars[k].thread, NULL, &car, &cars[k]);
        }
    }
     pthread_mutex_unlock(&map_mutex);
    refresh();
        sleep(1);
}
    endwin();
    return 0;
}
#define LEAVE_TOP {1,MIN_Y}
#define LEAVE_BOTTOM {-1,MAX_Y}
#define LEAVE_RIGHT {MAX_X,1}
#define LEAVE_LEFT {MIN_X,-1}

void* carTryMove(thread_context_t * ctx, int modX, int modY) {
    if (move_to(ctx,ctx->posX + modX, ctx->posY + modY, ctx->posX, ctx->posY)) {
        ctx->posX += modX;
        ctx->posY += modY;
        //printf("Przesuniecie do (%d,%d)\n",ctx->posX,ctx->posY);
    }
}
void* car(void *arg) {
    thread_context_t * ctx = arg;
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
    while(!place_at(ctx->posX, ctx->posY)) {
        sleep(1);
    }
    int destX = from_to[start][stop+1][0];
    int destY = from_to[start][stop+1][1];
    //printf("going from %d %d to %d %d\n", ctx->posX, ctx->posY, destX, destY);
    while (destX != ctx->posX || destX != ctx->posY) {
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
    ctx->alive = 0;
    
    
}