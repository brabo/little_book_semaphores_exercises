/*
 * All copyright and related rights waived via CC0.
 */

#ifndef SEM_H
#define SEM_H

int sem_postn(sem_t *s, int n);
int sem_waitn(sem_t *s, int n);

#endif
