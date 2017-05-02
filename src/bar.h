/*
 * All copyright and related rights waived via CC0.
 */

#ifndef BAR_H
#define BAR_H

struct bar {
	int n;
	int cnt;
	sem_t mtx;
	sem_t turn1;
	sem_t turn2;
};

int bar_init(struct bar *bar, int n);
int bar_wait(struct bar *bar, void (*critical)(void *args), void *args);
int bar_destroy(struct bar *bar);

#endif
