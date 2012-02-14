#include "map.h"

unsigned int index(int x, int y) {
    unsigned int realX = (unsigned int)(x+(-MIN_X));
    unsigned int realY = (unsigned int)(y+(-MIN_Y));
    return realX*TOTAL_Y+realY;
}

int place_at(int x, int y) {
    unsigned int i = index(x,y);
    map_lock();
    if (map[i] == 0) {
        map[i] = 1;
        map_unlock();
        return 1;
    } else {
        map_unlock();
        return 0;
    }
}

int remove_from(int x, int y) {
    unsigned int idx = index(x,y);
	map_lock();
	map[idx] = 0;
	map_unlock();
    return 1;
}

int move_to(int x, int y, int fromX, int fromY) {
    unsigned int idx = index(x,y);
    unsigned int from_idx = index(fromX,fromY);
	map_lock();
    if (x == SIDEWALK_LEFT && fromX < x && !lights->slot[L_CAR_LEFT]) {
        map_unlock();
        return 0;
    }
    if (x == SIDEWALK_RIGHT && fromX > x && !lights->slot[L_CAR_RIGHT]) {
        map_unlock();
        return 0;
    }
    if (y == SIDEWALK_TOP && fromY < y && !lights->slot[L_CAR_TOP]) {
        map_unlock();
        return 0;
    }
    if (y == SIDEWALK_BOTTOM && fromY > y && !lights->slot[L_CAR_BOTTOM]) {
        map_unlock();
        return 0;
    }
    if (map[idx] == 0) {
        map[idx] = 1;
        map[from_idx] = 0;
		map_unlock();
        return 1;
    } else {
		map_unlock();
        return 0;
    }
}



int shr_place_at(int x, int y) {
    unsigned int i = index(x,y);
    map_lock();
    if (map[i] <= 0) {
        map[i]--;
        map_unlock();
        return 1;
    } else {
        map_unlock();
        return 0;
    }
}

int shr_remove_from(int x, int y) {
    unsigned int idx = index(x,y);
	map_lock();
	map[idx]++;
	map_unlock();
    return 1;
}

int shr_move_to(int x, int y, int fromX, int fromY) {
    unsigned int idx = index(x,y);
    unsigned int from_idx = index(fromX,fromY);
	map_lock();
    if (x == LANE_LEFT && fromX < x) {
        if (fromY < 0 && !lights->slot[L_P_TOP_LEFT]) {
            map_unlock();
            return 0;
        } 
        if (fromY > 0 && !lights->slot[L_P_BOTTOM_LEFT]) {
            map_unlock();
            return 0;
        }             
    }
    if (x == LANE_RIGHT && fromX > x) {
        if (fromY < 0 && !lights->slot[L_P_TOP_RIGHT]) {
            map_unlock();
            return 0;
        } 
        if (fromY > 0 && !lights->slot[L_P_BOTTOM_RIGHT]) {
            map_unlock();
            return 0;
        }
    }
    if (y == LANE_TOP && fromY < y) {
        if (fromX < 0 && !lights->slot[L_P_LEFT_TOP]) {
            map_unlock();
            return 0;
        } 
        if (fromX > 0 && !lights->slot[L_P_RIGHT_TOP]) {
            map_unlock();
            return 0;
        }
    }
    if (y == LANE_BOTTOM && fromY > y) {
        if (fromX < 0 && !lights->slot[L_P_LEFT_BOTTOM]) {
            map_unlock();
            return 0;
        } 
        if (fromX > 0 && !lights->slot[L_P_RIGHT_BOTTOM]) {
            map_unlock();
            return 0;
        }
    }
    if (map[idx] <= 0) {
        map[idx] --;
        map[from_idx]++;
		map_unlock();
        return 1;
    } else {
		map_unlock();
        return 0;
    }
}

