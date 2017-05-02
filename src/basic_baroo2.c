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

#define MAX		10
#define REPEAT	10

struct critargs {
	int i;
	int j;
	char *string;
};

struct targs {
    int id;
    struct bar *bar;
};

void critic(void *args)
{
    struct critargs *critarg = (struct critargs *)args;
    printf("Critical section %i %i %s\n", critarg->i, critarg->j, critarg->string);
}

void *thread(void *arg)
{
    struct targs *targs = (struct targs *)arg;
    int i;

    for (i = 0; i < REPEAT; i++) {
        printf("Waiting..\n");
        struct critargs *critarg = malloc(1 * sizeof (struct critargs));
        critarg->i = i;
        critarg->j = targs->id;
        critarg->string = "testers";

        bar_wait(targs->bar, critic, critarg);
    }

}

int main(int argc, char **args)
{
    pthread_t tid[MAX];
    int i;

    int ret;

    struct targs *targs = malloc(MAX * sizeof (struct targs));
    struct bar *bar = malloc(1 * sizeof (struct bar));

    ret = bar_init(bar, MAX);
    printf("Barrier initialized ret == %d\r\n", ret);

    for (i = 0; i < MAX; i++) {
        targs[i].id = i;
        targs[i].bar = bar;
        ret = pthread_create(&tid[i], NULL, thread, &targs[i]);
        //printf("pthread_create returned %d\r\n", ret);
    }

    for (i = 0; i < MAX; i++) {
        pthread_join(tid[i], NULL);
    }

    ret = bar_destroy(bar);
    printf("Barrier destroyed ret == %d\r\n", ret);

    return 0;
}

