#include <unistd.h>
#include <string.h>
#include "map.h"
light_state mycycles[] = {
    {.slot={1,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1, 10}},
    {.slot={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 3}},
    {.slot={0,0,0,0, 1,1,0,0, 0,0,1,1, 0,0,0,0, 10}},
    {.slot={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 3}},
    {.slot={0,0,1,1, 0,0,0,0, 1,1,0,0, 0,0,0,0, 10}},
    {.slot={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 3}},
    {.slot={0,0,0,0, 0,0,1,1, 0,0,0,0, 1,1,0,0, 10}},
    {.slot={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 3}},
};
void change(int cycle) {
    map_lock();
    memcpy(lights, &mycycles[cycle], sizeof(light_state));
    map_unlock();
}
void* light_changer_ep(__attribute__((unused)) void *arg) {
    int cycle = 0;
    int time_left = mycycles[cycle].slot[L_LENGTH];
    change(0);
    while(1) {
        if (time_left <=0) {
            cycle = (cycle +1)%(sizeof(mycycles)/sizeof(light_state));
            time_left = mycycles[cycle].slot[L_LENGTH];
            change(cycle);
        } else {
            time_left--;
        }
        sleep(1);
    }
}
