/* 
 * Tracks which cells are in use
 *              -1 0 1
 *                 |r
 *                l|i
 *                e|g
 *                f|h
 *                t|t
 * -1         top  | lane
 *  0 -------------.----------------
 *  1      bottom  | lane
 *                l|l
 *                a|a
 *                n|n
 *                c|e
 */

#ifndef MAP_H
#define	MAP_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#include "config.h"
typedef enum {
    L_CAR_TOP, L_TRAM_TOP, L_P_TOP_LEFT, L_P_TOP_RIGHT,
    L_CAR_BOTTOM, L_TRAM_BOTTOM, L_P_BOTTOM_LEFT, L_P_BOTTOM_RIGHT,
    L_CAR_LEFT, L_TRAM_LEFT, L_P_LEFT_TOP, L_P_LEFT_BOTTOM,
    L_CAR_RIGHT, L_TRAM_RIGHT, L_P_RIGHT_TOP, L_P_RIGHT_BOTTOM,
    L_LENGTH, L_LAST
} light_slot_values;
typedef struct {
    int slot[L_LAST];
} light_state;

extern int *map;
extern light_state * lights;
void map_lock(void);
void map_unlock(void);
int place_at(int x, int y);
int remove_from(int x, int y);
int move_to(int x, int y, int fromX, int fromY);
int shr_place_at(int x, int y);
int shr_remove_from(int x, int y);
int shr_move_to(int x, int y, int fromX, int fromY);


#ifdef	__cplusplus
}
#endif

#endif	/* MAP_H */

