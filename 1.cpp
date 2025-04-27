#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstring>

#define NUMBER_OF_THREADS 12
#define Q 5

pthread_t tid[NUMBER_OF_THREADS];
pthread_mutex_t lock;

// Семафоры
sem_t sem_b, sem_e, sem_g, sem_h, sem_i, sem_k, sem_m, sem_n;

int err;

unsigned int lab2_thread_graph_id()
{
    return 5;
}

const char* lab2_unsynchronizedthreads()
{
    return "cdfg";
}

const char* lab2_sequentialthreads()
{
    return "ikm";
}

// ======== ПОТОКИ ========

void* thread_a(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'a' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    sem_post(&sem_b);
    sem_post(&sem_e);
    sem_post(&sem_g);
    return nullptr;
}

void* thread_b(void*) {
    sem_wait(&sem_b);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'b' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_c(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'c' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_d(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'd' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_e(void*) {
    sem_wait(&sem_e);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'e' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_f(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'f' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_g(void*) {
    sem_wait(&sem_g);
    for (int i = 0; i < 3 * Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    sem_post(&sem_h);
    return nullptr;
}

void* thread_h(void*) {
    sem_wait(&sem_h);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'h' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    sem_post(&sem_k);
    return nullptr;
}

void* thread_i(void*) {
    sem_wait(&sem_i);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'i' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    sem_post(&sem_k);
    return nullptr;
}

void* thread_k(void*) {
    sem_wait(&sem_k);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'k' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    sem_post(&sem_m);
    return nullptr;
}

void* thread_m(void*) {
    sem_wait(&sem_m);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    sem_post(&sem_n);
    return nullptr;
}

void* thread_n(void*) {
    sem_wait(&sem_n);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'n' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

// ======== ИНИЦИАЛИЗАЦИЯ ========

int lab2_init()
{
    pthread_mutex_init(&lock, NULL);

    sem_init(&sem_b, 0, 0);
    sem_init(&sem_e, 0, 0);
    sem_init(&sem_g, 0, 0);
    sem_init(&sem_h, 0, 0);
    sem_init(&sem_i, 0, 1); // i начинает первый в кольце
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);
    sem_init(&sem_n, 0, 0);

    // 1. a, c
    pthread_create(&tid[0], NULL, thread_a, NULL);
    pthread_create(&tid[2], NULL, thread_c, NULL);

    pthread_join(tid[0], NULL); // Ждём только 'a'

    // 2. b, e, g (после a)
    pthread_create(&tid[1], NULL, thread_b, NULL);
    pthread_create(&tid[4], NULL, thread_e, NULL);
    pthread_create(&tid[6], NULL, thread_g, NULL);

    pthread_join(tid[1], NULL);
    pthread_join(tid[4], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[6], NULL);

    // 3. d, f
    pthread_create(&tid[3], NULL, thread_d, NULL);
    pthread_create(&tid[5], NULL, thread_f, NULL);

    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);

    // 4. h
    pthread_create(&tid[7], NULL, thread_h, NULL);
    pthread_join(tid[7], NULL);

    // 5. i, k, m
    pthread_create(&tid[8], NULL, thread_i, NULL);
    pthread_create(&tid[9], NULL, thread_k, NULL);
    pthread_create(&tid[10], NULL, thread_m, NULL);

    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    pthread_join(tid[10], NULL);

    // 6. n
    pthread_create(&tid[11], NULL, thread_n, NULL);
    pthread_join(tid[11], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_b);
    sem_destroy(&sem_e);
    sem_destroy(&sem_g);
    sem_destroy(&sem_h);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);
    sem_destroy(&sem_n);

    return 0;
}
