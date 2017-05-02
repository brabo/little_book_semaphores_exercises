/*
 * Copyright (c) 2017 Centropy <contact@centropy.info>. All rights reserved.
 *
 * Authors: brabo
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdlib.h>
#include <semaphore.h>
#include "bar.h"
#include "xq.h"

int xq_init(struct xq *xq)
{
	sem_init(&xq->mtx, 0, 1);
	sem_init(&xq->lq, 0, 0);
	sem_init(&xq->fq, 0, 0);
	bar_init(&xq->bar, 2);

	return 0;
}

int xq_l(struct xq *xq, void (*critical)(void *args), void *args)
{
	sem_wait(&xq->mtx);

	if (xq->fllwrs > 0) {
		xq->fllwrs--;
		sem_post(&xq->fq);
	} else {
		xq->ldrs++;
		sem_post(&xq->mtx);
		sem_wait(&xq->lq);
	}

	if (critical) {
		critical(args);
	}

	bar_wait(&xq->bar, NULL, NULL);
	sem_post(&xq->mtx);

	return 0;
}

int xq_f(struct xq *xq, void (*critical)(void *args), void *args)
{
	sem_wait(&xq->mtx);

	if (xq->ldrs > 0) {
		xq->ldrs--;
		sem_post(&xq->lq);
	} else {
		xq->fllwrs++;
		sem_post(&xq->mtx);
		sem_wait(&xq->fq);
	}

	if (critical) {
		critical(args);
	}

	bar_wait(&xq->bar, NULL, NULL);

	return 0;
}

int xq_destroy(struct xq *xq)
{
	sem_destroy(&xq->mtx);
	sem_destroy(&xq->lq);
	sem_destroy(&xq->fq);
	bar_destroy(&xq->bar);

	return 0;
}
