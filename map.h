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
extern int *map;
void map_lock(void);
void map_unlock(void);
int place_at(int id, int x, int y);
int remove_from(int id, int x, int y);
int move_to(int id, int x, int y, int fromX, int fromY);


#ifdef	__cplusplus
}
#endif

#endif	/* MAP_H */

