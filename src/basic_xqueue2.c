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
#include "xq.h"

struct xq xq;

void critic_l(void *args)
{
    printf("Leader dances around.\n");
}

void critic_f(void *args)
{
    printf("Follower dances around.\n");
}

void *leader(void *arg)
{
    int i;

    printf("Leader enters the dancehall.\n");

    for (i = 0; i < 10; i++) {
        printf("Leader queues to dance.\n");

        xq_l(&xq, critic_l, NULL);

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

        xq_f(&xq, critic_f, NULL);

        printf("Follower leaves the dancefloor.\n");
    }

    printf("Follower leaves the dancehall.\n");

    pthread_exit(NULL);
}

int main(int argc, char **args)
{
    pthread_t lcons, fcons;

    int ret = xq_init(&xq);
    printf("eXclusive queue xq initialized ret == %d\r\n", ret);

    ret = pthread_create(&lcons, NULL, leader, NULL);
    printf("pthread_create leader returned %d\r\n", ret);

    ret = pthread_create(&fcons, NULL, follower, NULL);
    printf("pthread_create follower returned %d\r\n", ret);

    pthread_join(lcons, NULL);
    pthread_join(fcons, NULL);

    ret = xq_destroy(&xq);
    printf("eXclusive queue xq destroyed ret == %d\r\n", ret);

    return 0;
}

