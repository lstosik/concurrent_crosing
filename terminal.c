#include <curses.h>
#include "task.h"
#include "terminal.h"
#include "map.h"
#include "config.h"
extern task_context_t *cars;
void loop_graphic(void) {
	intrflush(stdscr, FALSE);

	int x, y, Pid, i;

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
		for (int k = 0; k < MAX_CARS; k++) {
			if (cars[k].alive) {
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
	cbreak();
	while (getch() == ERR) {
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
