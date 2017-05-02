/*
 * All copyright and related rights waived via CC0.
 */

#ifndef MUX_H
#define MUX_H

int mux_init(sem_t *mux, int n);
int mux_wait(sem_t *mux);
int mux_destroy(sem_t *mux);

#endif
