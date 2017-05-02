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

sem_t mtx, turn1, turn2;
int cnt = 0;

void *add(void *arg)
{
    int i;

    for (i = 0; i < 10; i++) {
        printf("Waiting..\n");

        sem_wait(&mtx);
        cnt = cnt + 1;
        if (cnt == 10) {
            printf("Barrier reached!\n");
            //sem_wait(&turn2);
            int j;
            for (j = 0; j < 10; j++) {
                sem_post(&turn1);
            }
        }
        sem_post(&mtx);

        sem_wait(&turn1);
//        sem_post(&turn1);

        printf("critical section\n");

        sem_wait(&mtx);
        cnt = cnt - 1;
        if (cnt == 0) {
            printf("Locking again!\n");
            //sem_wait(&turn1);
            int j;
            for (j = 0; j < 10; j++) {
                sem_post(&turn2);
            }
        }
        sem_post(&mtx);

        sem_wait(&turn2);
        //sem_post(&turn2);
    }

}


int main(int argc, char **args)
{
    pthread_t tid[MAX];
    int i;

    int ret = sem_init(&mtx, 0, 1);
    printf("Semaphore mtx initialized ret == %d\r\n", ret);


    ret = sem_init(&turn1, 0, 0);
    printf("Semaphore turn1 initialized ret == %d\r\n", ret);

    ret = sem_init(&turn2, 0, 0);
    printf("Semaphore turn2 initialized ret == %d\r\n", ret);

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

