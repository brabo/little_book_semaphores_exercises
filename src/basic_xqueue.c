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
#include <semaphore.h>
#include "bar.h"

sem_t lqueue, fqueue, mtx;
int ldrs, fllwrs = 0;
struct bar bar;

void *leader(void *arg)
{
    int i;

    printf("Leader enters the dancehall.\n");

    for (i = 0; i < 10; i++) {
        printf("Leader queues to dance.\n");

        sem_wait(&mtx);

        if (fllwrs > 0) {
            fllwrs--;
            sem_post(&fqueue);
        } else {
            ldrs++;
            sem_post(&mtx);
            sem_wait(&lqueue);
        }

        printf("Leader dances around.\n");

        bar_wait(&bar, NULL, NULL);
        sem_post(&mtx);

        printf("Leader leaves the dancefloor.\n");

    }

    printf("Leader leaves the dancehall.\n");

    pthread_exit(NULL);
}

void *follower(void *arg)
{
    int i;

    printf("Follower enters the dancehall.\n");

    for (i = 0; i < 10; i++) {
        printf("Follower queues to dance.\n");

        sem_wait(&mtx);

        if (ldrs > 0) {
            ldrs--;
            sem_post(&lqueue);
        } else {
            fllwrs++;
            sem_post(&mtx);
            sem_wait(&fqueue);
        }

        printf("Follower dances around.\n");

        bar_wait(&bar, NULL, NULL);

        printf("Follower leaves the dancefloor.\n");

    }

    printf("Follower leaves the dancehall.\n");

    pthread_exit(NULL);
}

int main(int argc, char **args)
{
    pthread_t lcons, fcons;

    int ret = sem_init(&lqueue, 0, 0);
    printf("Semaphore lqueue initialized ret == %d\r\n", ret);

    ret = sem_init(&fqueue, 0, 0);
    printf("Semaphore fqueue initialized ret == %d\r\n", ret);

    ret = sem_init(&mtx, 0, 1);
    printf("Semaphore mtx initialized ret == %d\r\n", ret);

    ret = bar_init(&bar, 2);
    printf("Barrier bar initialized ret == %d\r\n", ret);

    ret = pthread_create(&lcons, NULL, leader, NULL);
    printf("pthread_create leader returned %d\r\n", ret);

    ret = pthread_create(&fcons, NULL, follower, NULL);
    printf("pthread_create follower returned %d\r\n", ret);

    pthread_join(lcons, NULL);
    pthread_join(fcons, NULL);

    ret = sem_destroy(&lqueue);
    printf("Semaphore lqueue destroyed ret == %d\r\n", ret);

    ret = sem_destroy(&fqueue);
    printf("Semaphore fqueue destroyed ret == %d\r\n", ret);

    ret = sem_destroy(&mtx);
    printf("Semaphore mtx destroyed ret == %d\r\n", ret);

    ret = bar_destroy(&bar);
    printf("Barrier bar destroyed ret == %d\r\n", ret);


    return 0;
}

