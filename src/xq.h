/*
 * All copyright and related rights waived via CC0.
 */

#ifndef XQ_H
#define XQ_H

#include "bar.h"

struct xq {
    sem_t mtx;
    sem_t lq;
    sem_t fq;
    int ldrs;
    int fllwrs;
    struct bar bar;
};

int xq_init(struct xq *xq);
int xq_l(struct xq *xq, void (*critical)(void *args), void *args);
int xq_f(struct xq *xq, void (*critical)(void *args), void *args);
int xq_destroy(struct xq *xq);

#endif
