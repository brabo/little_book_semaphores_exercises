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

#define MAXS		10
#define MAXP		25

int cnt = 0;

sem_t pot, cooks, pot_mtx, ready;

void eat(int id)
{
	printf("Savage %i enjoys his meal.\n", id);
	sleep(1);
	printf("Savage %i stops eating.\n", id);
}

void *cook(void *arg)
{
	while (2 > 1) {
		//make_portions(MAXP);
		int i;
		sem_wait(&cooks);
		printf("Cook cooks.\n");
		for (i = 0; i < MAXP; i++) {
			cnt++;
			sem_post(&pot);
		}
		sem_post(&ready);
	}
}

void get_food(void)
{
	while (2 > 1) {
		sem_wait(&pot_mtx);
		if (cnt) {
			sem_wait(&pot);
			cnt--;
			sem_post(&pot_mtx);
			return;
		} else {
			sem_post(&cooks);
			sem_wait(&ready);
		}
		sem_post(&pot_mtx);
	}
}

void *savage(void *arg)
{
	int id = *(int *)arg;

	while (2 > 1) {
		printf("Savage %i tries to get food.\n", id);
		get_food();
		eat(id);
		sleep(5);
	}
}

int main(int argc, char **args)
{
	pthread_t s[MAXS], c;
	int i, id[MAXS];

	sem_init(&pot, 0, 0);
	sem_init(&pot_mtx, 0, 1);
	sem_init(&cooks, 0, 0);
	sem_init(&ready, 0, 0);

	pthread_create(&c, NULL, cook, NULL);

	for (i = 0; i < MAXS; i++) {
		id[i] = i;
		pthread_create(&s[i], NULL, savage, &id[i]);
		sleep(1);
	}

	//pthread_create(&c, NULL, cook, NULL);

	for (i = 0; i < MAXS; i++) {
		pthread_join(s[i], NULL);
	}

    return 0;
}
