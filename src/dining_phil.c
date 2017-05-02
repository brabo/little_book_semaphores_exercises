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

#define MAXPH		5

sem_t forks[MAXPH];

int forks_get(int id)
{
	int n;

	if (id == 4) {
		n = 0;
		sem_wait(&forks[n]);
		sem_wait(&forks[id]);
	} else {
		n = id + 1;
		sem_wait(&forks[id]);
		sem_wait(&forks[n]);
	}

	return 0;
}

int forks_drop(int id)
{
	int n;

	if (id == 4) {
		n = 0;
	} else {
		n = id + 1;
	}

	sem_post(&forks[id]);
	sem_post(&forks[n]);

	return 0;
}

void think(int id)
{
	//printf("Philosopher #%i thinks deeply..\n", id);
	sleep(1);
}

void eat(int id)
{
	printf("Philosopher %i enjoys his meal.\n", id);
	sleep(1);
	printf("Philosopher %i stops eating.\n", id);
}

void *phil(void *arg)
{
	int id = *(int *)arg;

	while (2 > 1) {
		think(id);
		forks_get(id);
		eat(id);
		forks_drop(id);
	}
}

int main(int argc, char **args)
{
	pthread_t p[MAXPH];
	int i, id[MAXPH];

	for (i = 0; i < MAXPH; i++) {
		sem_init(&forks[i], 0, 1);
	}

	for (i = 0; i < MAXPH; i++) {
		id[i] = i;
		pthread_create(&p[i], NULL, phil, &id[i]);
	}

	for (i = 0; i < MAXPH; i++) {
		pthread_join(p[i], NULL);
	}

    return 0;
}
