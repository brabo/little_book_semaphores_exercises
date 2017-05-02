/*
 * All copyright and related rights waived via CC0.
 */


#ifndef LSWITCH_H
#define LSWITCH_H

struct lswitch {
	int cnt;
	sem_t mtx;
};

int lswitch_init(struct lswitch *ls);
int lswitch_lock(struct lswitch *ls, sem_t *s);
int lswitch_unlock(struct lswitch *ls, sem_t *s);
int lswitch_destroy(struct lswitch *ls);

#endif
