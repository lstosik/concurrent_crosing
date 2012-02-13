#include <curses.h>
#include <unistd.h>
#include "task.h"
#include "terminal.h"
#include "map.h"
#include "config.h"
extern task_context_t *cars;

void dash_h(int start_x, int stop_x, int y) {
    unsigned r_start_x = start_x + (-MIN_X);
    unsigned r_stop_x = stop_x + (-MIN_X);
    unsigned r_y = y + (-MIN_Y);
    
    for (unsigned int j = r_start_x; j <= r_stop_x; j++) {
        mvprintw(r_y, j * 2, "- ");
    }
}

void dash_v(int x, int start_y, int stop_y) {
    unsigned r_start_y = start_y + (-MIN_Y);
    unsigned r_stop_y = stop_y + (-MIN_Y);
    unsigned r_x = x + (-MIN_X);
    for (unsigned int j = r_start_y; j <= r_stop_y; j++) {
        mvprintw(j, 2*r_x, (j % 2) ? "| " : " |");
    }
}

void line_h(int start_x, int stop_x, int y, char * str) {
    unsigned r_start_x = start_x + (-MIN_X);
    unsigned r_stop_x = stop_x + (-MIN_X);
    unsigned r_y = y + (-MIN_Y);
    
    for (unsigned int j = r_start_x; j <= r_stop_x; j++) {
        mvprintw(r_y, j * 2, str);
    }
}

void line_v(int x, int start_y, int stop_y, char * str) {
    unsigned r_start_y = start_y + (-MIN_Y);
    unsigned r_stop_y = stop_y + (-MIN_Y);
    unsigned r_x = x + (-MIN_X);
    for (unsigned int j = r_start_y; j <= r_stop_y; j++) {
        mvprintw(j, 2*r_x, str);
    }
}
void loop_graphic(void) {
	intrflush(stdscr, FALSE);

	int x, y;

	//
	initscr();
	getmaxyx(stdscr, y, x);

	if ((y < 20) || (x < 60)) {
		endwin();
		return;
	}

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_GREEN);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_BLACK);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_BLACK, COLOR_YELLOW);
	init_pair(7, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(8, COLOR_BLACK, COLOR_CYAN);
	init_pair(9, COLOR_WHITE, COLOR_BLACK);
	init_pair(10, COLOR_BLACK, COLOR_RED);
	init_pair(11, COLOR_RED, COLOR_MAGENTA);
	init_pair(12, COLOR_WHITE, COLOR_WHITE);
	nodelay(stdscr, 1);
	cbreak();
	curs_set(0);
	while (getch() == ERR) {
		clear();
		map_lock();
		attron(COLOR_PAIR(2));
		for (int tx = MIN_X; tx <= MAX_X; tx++) {
			for (int ty = MIN_Y; ty <= MAX_Y; ty++) {
				int realX = tx + (-MIN_X);
				int realY = ty + (-MIN_Y);
				mvprintw(realY, 2 * realX, "%02d", map[realX * TOTAL_Y + realY]);
			}
		}
        attron(COLOR_PAIR(8));
        line_v(SIDEWALK_LEFT, MIN_Y, MAX_Y, "  ");
        line_v(SIDEWALK_RIGHT, MIN_Y, MAX_Y, "  ");
        line_h(MIN_X, MAX_X, SIDEWALK_TOP, "  ");
        line_h(MIN_X, MAX_X, SIDEWALK_BOTTOM, "  ");
        attron(COLOR_PAIR(7));
        line_v(LANE_LEFT, MIN_Y, MAX_Y, "  ");
        line_v(LANE_RIGHT, MIN_Y, MAX_Y, "  ");
        line_h(MIN_X, MAX_X, LANE_TOP, "  ");
        line_h(MIN_X, MAX_X, LANE_BOTTOM, "  ");
        line_v(TRAM_LANE_LEFT, MIN_Y, MAX_Y, "||");
        line_v(TRAM_LANE_RIGHT, MIN_Y, MAX_Y, "||");
        line_h(MIN_X, MAX_X, TRAM_LANE_TOP, "==");
        line_h(MIN_X, MAX_X, TRAM_LANE_BOTTOM, "==");
        attron(COLOR_PAIR(5));
        dash_h(MIN_X, LANE_LEFT - 1, 0);
        dash_h(LANE_RIGHT + 1, MAX_X, 0);
        dash_v(0, MIN_Y, LANE_TOP - 1);
        dash_v(0, LANE_BOTTOM + 1, MAX_Y);
		for (int k = 0; k < MAX_CARS; k++) {
			if (cars[k].alive) {
                if (cars[k].type==tram) {
                    attron(COLOR_PAIR(7));
                } else if (cars[k].type==pedestriant) {
                    attron(COLOR_PAIR(9));
                } else {
                    attron(COLOR_PAIR(6));
                }
				mvprintw((cars[k].posY + (-MIN_Y)), 2 * (cars[k].posX + (-MIN_X)), "%02d", k);
			} else {
				task_start(k);
			}
		}
		map_unlock();
		refresh();
		sleep(1);
	}
	endwin();
}

void loop(void) {
	//nodelay(stdscr, 1);
	//cbreak();
	while (1) {
		map_lock();
		for (int k = 0; k < MAX_CARS; k++) {
			if (!cars[k].alive) {
				task_start(k);
			}
		}
		map_unlock();
		sleep(1);
	}
}
