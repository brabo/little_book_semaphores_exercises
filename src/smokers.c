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
#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>

sem_t agent, tobacco, paper, match, se1, se2, se3, p_mtx;
bool ist, isp, ism = false;

void smoke(int id)
{
	printf("Smoker %i smokes..\n", id);
	sleep(2);
}

void roll(void)
{
	sleep(1);
}

void *a1(void *arg)
{
	while (2 > 1) {
		sem_wait(&agent);
		sem_post(&tobacco);
		sem_post(&paper);
	}
}

void *a2(void *arg)
{
	while (2 > 1) {
		sem_wait(&agent);
		sem_post(&paper);
		sem_post(&match);
	}
}

void *a3(void *arg)
{
	while (2 > 1) {
		sem_wait(&agent);
		sem_post(&match);
		sem_post(&tobacco);
	}
}

void *p1(void *arg)
{
	while (2 > 1) {
		sem_wait(&tobacco);
		sem_wait(&p_mtx);
		if (isp) {
			isp = false;
			sem_post(&se1);
		} else if (ism) {
			ism = false;
			sem_post(&se3);
		} else {
			ist = true;
		}
		sem_post(&p_mtx);
	}
}

void *p2(void *arg)
{
	while (2 > 1) {
		sem_wait(&paper);
		sem_wait(&p_mtx);
		if (ism) {
			ism = false;
			sem_post(&se2);
		} else if (ist) {
			ist = false;
			sem_post(&se1);
		} else {
			isp = true;
		}
		sem_post(&p_mtx);
	}
}

void *p3(void *arg)
{
	while (2 > 1) {
		sem_wait(&match);
		sem_wait(&p_mtx);
		if (ist) {
			ist = false;
			sem_post(&se3);
		} else if (isp) {
			isp = false;
			sem_post(&se2);
		} else {
			ism = true;
		}
		sem_post(&p_mtx);
	}
}

void *s1(void *arg)
{
	int id = *(int *)arg;
	while (2 > 1) {
		sem_wait(&se1);
		roll();
		sem_post(&agent);
		smoke(id);
	}
}

void *s2(void *arg)
{
	int id = *(int *)arg;
	while (2 > 1) {
		sem_wait(&se2);
		roll();
		sem_post(&agent);
		smoke(id);
	}
}

void *s3(void *arg)
{
	int id = *(int *)arg;
	while (2 > 1) {
		sem_wait(&se3);
		roll();
		sem_post(&agent);
		smoke(id);
	}
}

int main(int argc, char **args)
{
	pthread_t a[3], p[3], s[3];
	int id[3];
	id[0] = 0;
	id[1] = 1;
	id[2] = 2;

	sem_init(&agent, 0, 1);
	sem_init(&tobacco, 0, 1);
	sem_init(&paper, 0, 1);
	sem_init(&match, 0, 1);
	sem_init(&se1, 0, 1);
	sem_init(&se2, 0, 1);
	sem_init(&se3, 0, 1);
	sem_init(&p_mtx, 0, 1);

	pthread_create(&a[0], NULL, a1, NULL);
	pthread_create(&a[1], NULL, a2, NULL);
	pthread_create(&a[2], NULL, a3, NULL);

	pthread_create(&p[0], NULL, p1, NULL);
	pthread_create(&p[1], NULL, p2, NULL);
	pthread_create(&p[2], NULL, p3, NULL);

	pthread_create(&s[0], NULL, s1, &id[0]);
	pthread_create(&s[1], NULL, s2, &id[1]);
	pthread_create(&s[2], NULL, s3, &id[2]);



	pthread_join(s[0], NULL);
	pthread_join(s[1], NULL);
	pthread_join(s[2], NULL);

	pthread_join(p[0], NULL);
	pthread_join(p[1], NULL);
	pthread_join(p[2], NULL);

	pthread_join(a[0], NULL);
	pthread_join(a[1], NULL);
	pthread_join(a[2], NULL);

    return 0;
}

