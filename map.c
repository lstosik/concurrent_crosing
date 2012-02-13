#include "map.h"


int place_at(int id, int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    map_lock();
    if (map[realX*TOTAL_Y+realY] == 0) {
        map[realX*TOTAL_Y+realY] = id;
        map_unlock();
        return 1;
    } else {
        map_unlock();
        return 0;
    }
}

int remove_from(int id, int x, int y) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
	map_lock();
	map[realX*TOTAL_Y+realY] = 0;
	/*map_lock();
	if (map[realX*TOTAL_Y+realY] == id) {
		map[realX*TOTAL_Y+realY] = 0;
	} else {
		// TODO: maybe put assert here?
		map_unlock();
		return 0;
	}*/
	map_unlock();
    return 1;
}

int move_to(int id, int x, int y, int fromX, int fromY) {
    unsigned int realX = x+(-MIN_X);
    unsigned int realY = y+(-MIN_Y);
    unsigned int realFromX = fromX+(-MIN_X);
    unsigned int realFromY = fromY+(-MIN_Y);
	map_lock();
    //if (!map[realFromX][realFromY]) {
        //fprintf(stderr,"blad przy ruszaniu z %d, %d do %d,%d, pole zrodlowe nie jest zajete\n",fromX,fromY,x,y);
        //exit(0);
    //};
    if (map[realX*TOTAL_Y+realY] == 0) {
        map[realX*TOTAL_Y+realY] = id;
        map[realFromX*TOTAL_Y+realFromY] = 0;
		map_unlock();
        return 1;
    } else {
		map_unlock();
        return 0;
    }
}
