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

sem_t adone;

void *a(void *arg)
{
    printf("Thread A says ohai thar.\n");
    sem_post(&adone);
    pthread_exit(NULL);
}

void *b(void *arg)
{
    sem_wait(&adone);
    printf("Thread B says ohai thar.\n");
    pthread_exit(NULL);
}


int main(int argc, char **args)
{
    pthread_t acons, bcons;

    int ret = sem_init(&adone, 0, 0);
    printf("Semaphore initialized ret == %d\r\n", ret);

    ret = pthread_create(&bcons, NULL, b, &adone);
    printf("pthread_create b returned %d\r\n", ret);

    ret = pthread_create(&acons, NULL, a, &adone);
    printf("pthread_create a returned %d\r\n", ret);

    pthread_join(acons, NULL);
    pthread_join(bcons, NULL);

    ret = sem_destroy(&adone);
    printf("Semaphore destroyed ret == %d\r\n", ret);

    return 0;
}

