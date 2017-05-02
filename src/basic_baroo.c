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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/time.h>

#define MAX		10
#define REPEAT	10

struct baroo {
	int n;
	int cnt;
	sem_t mtx;
	sem_t turn1;
	sem_t turn2;
	void (*critical)(void *args);
	void *args;
};

int bar_init(struct baroo *bar, int n, void (*critical)(void *args), void *args)
{
	bar->n = n;
	bar->cnt = 0;
	bar->critical = critical;
	bar->args = args;

	sem_init(&bar->mtx, 0, 1);
	sem_init(&bar->turn1, 0, 0);
	sem_init(&bar->turn2, 0, 0);

	return 0;
}

int sem_turn(sem_t *turn, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		sem_post(turn);
	}

	return 0;
}

int bar_phase1(struct baroo *bar)
{
	sem_wait(&bar->mtx);

	bar->cnt = bar->cnt + 1;

	if (bar->cnt == bar->n) {
		printf("Barrier reached!\n");
		sem_turn(&bar->turn1, bar->n);
	}

	sem_post(&bar->mtx);
	sem_wait(&bar->turn1);

	return 0;
}

int bar_phase2(struct baroo *bar)
{
	sem_wait(&bar->mtx);

	bar->cnt = bar->cnt - 1;

	if (bar->cnt == 0) {
		printf("Locking again!\n");
		sem_turn(&bar->turn2, bar->n);
	}

	sem_post(&bar->mtx);
	sem_wait(&bar->turn2);

	return 0;
}

int bar_wait(struct baroo *bar)
{
	bar_phase1(bar);
	bar->critical(bar->args);
	bar_phase2(bar);

	return 0;
}

int bar_destroy(struct baroo *bar)
{
	sem_destroy(&bar->mtx);
	sem_destroy(&bar->turn1);
	sem_destroy(&bar->turn2);

	return 0;
}

struct baroo barrier;

void *add(void *arg)
{
    int i;

    for (i = 0; i < REPEAT; i++) {
        printf("Waiting..\n");

        bar_wait(&barrier);
    }

}

struct critargs {
	int i;
	int j;
	char *string;
};

void critic(void *args)
{
	struct critargs *critarg = (struct critargs *)args;
	printf("Critical section %i %i %s\n", critarg->i, critarg->j, critarg->string);
}

int main(int argc, char **args)
{
    pthread_t tid[MAX];
    int i;

    int ret;
    struct critargs *critarg = malloc(1 * sizeof (struct critargs));
    critarg->i = 42;
    critarg->j = 24;
    critarg->string = "testers";

    ret = bar_init(&barrier, MAX, critic, critarg);
    printf("Barrier bar initialized ret == %d\r\n", ret);

    for (i = 0; i < MAX; i++) {
        ret = pthread_create(&tid[i], NULL, add, NULL);
        //printf("pthread_create returned %d\r\n", ret);
    }

    for (i = 0; i < MAX; i++) {
        pthread_join(tid[i], NULL);
    }

    ret = bar_destroy(&barrier);
    printf("Semaphore destroyed ret == %d\r\n", ret);

    return 0;
}

