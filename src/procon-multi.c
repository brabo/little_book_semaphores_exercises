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
#include "rb.h"
#include "sem.h"

#define BUF_SIZE	18640
#define MAXTH		1

struct ring *ebuf;
struct ring *ibuf;
sem_t event, consume, ebuf_mtx, ibuf_mtx, efrees, ifrees, ef_mtx, if_mtx;

void *producer(void *arg)
{
	while (2 > 1) {
		int cnt;
		sem_wait(&event);

		cnt = rb_used(ebuf);
		sem_wait(&if_mtx);
		sem_waitn(&ifrees, cnt);
		sem_post(&if_mtx);

		sem_wait(&ebuf_mtx);
		sem_wait(&ibuf_mtx);

		rb_x(ebuf, ibuf, cnt);

		sem_post(&ebuf_mtx);
		sem_post(&ibuf_mtx);

		sem_wait(&ef_mtx);
		sem_postn(&efrees, cnt);
		sem_post(&ef_mtx);
		sem_post(&consume);
	}
}

void *consumer(void *arg)
{
	while (2 > 1) {
		int cnt;
		sem_wait(&consume);

		cnt = rb_used(ibuf);
		char *output = malloc(cnt * sizeof (char));

		sem_wait(&ibuf_mtx);
		rb_read(ibuf, output, cnt);
		sem_post(&ibuf_mtx);

		sem_wait(&if_mtx);
		sem_postn(&ifrees, cnt);
		sem_post(&if_mtx);

		printf("%s", output);
	}
}

void *generator(void *arg)
{
	volatile int i = 0;

	while (2 > 1) {
		int len;
		char *input = malloc(16 * sizeof (char));
		sprintf(input, "Run %i\n", i++);

		len = strlen(input) + 1;
		sem_wait(&ef_mtx);
		sem_waitn(&efrees, len);
		sem_post(&ef_mtx);

		sem_wait(&ebuf_mtx);
		rb_write(ebuf, input, len);
		sem_post(&ebuf_mtx);

		sem_post(&event);
		usleep(1);
	}
}

int main(int argc, char **args)
{
	pthread_t p[MAXTH], c[MAXTH], g[MAXTH];
	int i;

	ebuf = rb_init(BUF_SIZE);
	ibuf = rb_init(BUF_SIZE);

	sem_init(&event, 0, 0);
	sem_init(&consume, 0, 0);
	sem_init(&ebuf_mtx, 0, 1);
	sem_init(&ibuf_mtx, 0, 1);
	sem_init(&ifrees, 0, BUF_SIZE);
	sem_init(&efrees, 0, BUF_SIZE);
	sem_init(&ef_mtx, 0, 1);
	sem_init(&if_mtx, 0, 1);

	for (i = 0; i < MAXTH; i++) {
		pthread_create(&c[i], NULL, consumer, NULL);
		pthread_create(&p[i], NULL, producer, NULL);
		//usleep(100000);
		//pthread_create(&g[i], NULL, generator, NULL);
	}

	sleep(1);
	pthread_create(&g[0], NULL, generator, NULL);

	pthread_join(g[0], NULL);

	for (i = 0; i < MAXTH; i++) {
		pthread_join(p[i], NULL);
		pthread_join(c[i], NULL);
	}

    return 0;
}
