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

#define MAX 10

sem_t mtx, bar;
int cnt = 0;

void *add(void *arg)
{
    printf("Waiting..\n");
    sem_wait(&mtx);
    cnt = cnt + 1;
    //printf("count == %i\n", cnt);
    sem_post(&mtx);

    if (cnt == 10) {
        printf("Barrier reached!\n");
        sem_post(&bar);
    }

    sem_wait(&bar);
    sem_post(&bar);

    //sem_post(&mtx);
    printf("critical section\n");
}


int main(int argc, char **args)
{
    pthread_t tid[MAX];
    int i;

    int ret = sem_init(&mtx, 0, 1);
    printf("Semaphore mtx initialized ret == %d\r\n", ret);

    ret = sem_init(&bar, 0, 0);
    printf("Semaphore bar initialized ret == %d\r\n", ret);

    printf("count == %i\n", cnt);

    for (i = 0; i < MAX; i++) {
        ret = pthread_create(&tid[i], NULL, add, NULL);
        //printf("pthread_create returned %d\r\n", ret);
    }

    for (i = 0; i < MAX; i++) {
        pthread_join(tid[i], NULL);
    }

    ret = sem_destroy(&mtx);
    printf("Semaphore destroyed ret == %d\r\n", ret);

    printf("count == %i\n", cnt);

    return 0;
}

