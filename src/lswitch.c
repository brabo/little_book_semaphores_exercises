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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "lswitch.h"

int lswitch_init(struct lswitch *ls)
{
	ls->cnt = 0;
	sem_init(&ls->mtx, 0, 1);

	return 0;
}

int lswitch_lock(struct lswitch *ls, sem_t *s)
{
	sem_wait(&ls->mtx);
	ls->cnt++;
	if (ls->cnt == 1) {
		sem_wait(s);
	}
	sem_post(&ls->mtx);
}

int lswitch_unlock(struct lswitch *ls, sem_t *s)
{
	sem_wait(&ls->mtx);
	ls->cnt--;
	if (ls->cnt == 0) {
		sem_post(s);
	}
	sem_post(&ls->mtx);
}

int lswitch_destroy(struct lswitch *ls)
{
	ls->cnt = 0;
	sem_destroy(&ls->mtx);
}

