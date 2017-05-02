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

#define MAXTH		1

sem_t rdrs_mtx, empty;
int rdrs = 0;

void *reader(void arg)
{
	sem_wait(&rdrs_mtx);
	rdrs++;
	if (rdrs == 1) {
		sem_wait(&empty);
	}
	sem_post(&rdrs_mtx);

	// do reading stuffs

	sem_wait(&rdrs_mtx);
	rdrs--;
	if (rdrs == 0) {
		sem_post(empty);
	}
	sem_post(&rdrs_mtx);
}

void *writer(void arg)
{
	sem_wait(&empty);

	// do writing stuffs

	sem_post(&empty);
}

int main(int argc, char **args)
{
	pthread_t r[MAXTH], w[MAXTH];
	int i;

	sem_init(&rdrs_mtx, 0, 1);
	sem_init(&empty, 0, 1);

	for (i = 0; i < MAXTH; i++) {
		pthread_create(&r[i], NULL, reader, NULL);
		pthread_create(&w[i], NULL, writer, NULL);
	}

	for (i = 0; i < MAXTH; i++) {
		pthread_join(w[i], NULL);
		pthread_join(r[i], NULL);
	}

    return 0;
}


