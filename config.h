/* 
 * Constants, macros and parameters
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
	
#define MIN_X -10
#define MIN_Y -10
#define MAX_X 10
#define MAX_Y 10
#define MAX_CARS 30
#define TOTAL_X -(MIN_X-MAX_X-1)
#define TOTAL_Y -(MIN_Y-MAX_Y-1)

#define LANE_TOP -2
#define LANE_BOTTOM 2
#define LANE_RIGHT 2
#define LANE_LEFT -2
    
#define TRAM_LANE_TOP -1
#define TRAM_LANE_BOTTOM 1
#define TRAM_LANE_RIGHT 1
#define TRAM_LANE_LEFT -1
    
#define SIDEWALK_TOP -4
#define SIDEWALK_BOTTOM 4
#define SIDEWALK_RIGHT 4
#define SIDEWALK_LEFT -4

	
#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

