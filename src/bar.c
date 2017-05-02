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

#include <semaphore.h>
#include "bar.h"
#include "sem.h"

int bar_init(struct bar *bar, int n)
{
	bar->n = n;
	bar->cnt = 0;

	sem_init(&bar->mtx, 0, 1);
	sem_init(&bar->turn1, 0, 0);
	sem_init(&bar->turn2, 0, 0);

	return 0;
}

static int bar_phase1(struct bar *bar)
{
	sem_wait(&bar->mtx);

	bar->cnt = bar->cnt + 1;

	if (bar->cnt == bar->n) {
		sem_postn(&bar->turn1, bar->n);
	}

	sem_post(&bar->mtx);
	sem_wait(&bar->turn1);

	return 0;
}

static int bar_phase2(struct bar *bar)
{
	sem_wait(&bar->mtx);

	bar->cnt = bar->cnt - 1;

	if (bar->cnt == 0) {
		sem_postn(&bar->turn2, bar->n);
	}

	sem_post(&bar->mtx);
	sem_wait(&bar->turn2);

	return 0;
}

int bar_wait(struct bar *bar, void (*critical)(void *args), void *args)
{
	bar_phase1(bar);
	if (critical) {
		critical(args);
	}
	bar_phase2(bar);

	return 0;
}

int bar_destroy(struct bar *bar)
{
	sem_destroy(&bar->mtx);
	sem_destroy(&bar->turn1);
	sem_destroy(&bar->turn2);

	return 0;
}
