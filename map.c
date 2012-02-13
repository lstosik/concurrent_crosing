#include "map.h"


int place_at(int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    map_lock();
    if (map[realX*TOTAL_Y+realY] == 0) {
        map[realX*TOTAL_Y+realY] = 1;
        map_unlock();
        return 1;
    } else {
        map_unlock();
        return 0;
    }
}

int remove_from(int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
	map_lock();
	map[realX*TOTAL_Y+realY] = 0;
	map_unlock();
    return 1;
}

int move_to(int x, int y, int fromX, int fromY) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    unsigned int realFromX = fromX+(-MIN_X);
    unsigned int realFromY = fromY+(-MIN_Y);
	map_lock();
    if (map[realX*TOTAL_Y+realY] == 0) {
        map[realX*TOTAL_Y+realY] = 1;
        map[realFromX*TOTAL_Y+realFromY] = 0;
		map_unlock();
        return 1;
    } else {
		map_unlock();
        return 0;
    }
}



int shr_place_at(int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    map_lock();
    if (map[realX*TOTAL_Y+realY] <= 0) {
        map[realX*TOTAL_Y+realY]--;
        map_unlock();
        return 1;
    } else {
        map_unlock();
        return 0;
    }
}

int shr_remove_from(int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
	map_lock();
	map[realX*TOTAL_Y+realY]++;
	map_unlock();
    return 1;
}

int shr_move_to(int x, int y, int fromX, int fromY) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    unsigned int realFromX = fromX+(-MIN_X);
    unsigned int realFromY = fromY+(-MIN_Y);
	map_lock();
    if (map[realX*TOTAL_Y+realY] <= 0) {
        map[realX*TOTAL_Y+realY] --;
        map[realFromX*TOTAL_Y+realFromY]++;
		map_unlock();
        return 1;
    } else {
		map_unlock();
        return 0;
    }
}