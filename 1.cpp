#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstring>

#define NUM_THREADS 12
#define Q 5

pthread_t tid[NUM_THREADS];
pthread_mutex_t lock;
sem_t sem_i, sem_k, sem_m;
int err;

unsigned int lab2_thread_graph_id() { return 5; }
const char* lab2_unsynchronized_threads() { return "cdfg"; }
const char* lab2_sequential_threads() { return "ikm"; }

// ========== ПОТОКИ ==========
void* run_thread(const char c) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << c << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}
void* thread_a(void*) { return run_thread('a'); }
void* thread_b(void*) { return run_thread('b'); }
void* thread_c(void*) { return run_thread('c'); }
void* thread_d(void*) { return run_thread('d'); }
void* thread_e(void*) { return run_thread('e'); }
void* thread_f(void*) { return run_thread('f'); }
void* thread_g(void*) { return run_thread('g'); }
void* thread_h(void*) { return run_thread('h'); }
void* thread_n(void*) { return run_thread('n'); }

// ======= СИНХРОНИЗИРОВАННЫЕ =======
void* thread_i(void*) {
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_i);
        pthread_mutex_lock(&lock);
        std::cout << "i" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_k);
    }
    return nullptr;
}
void* thread_k(void*) {
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_k);
        pthread_mutex_lock(&lock);
        std::cout << "k" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_m);
    }
    return nullptr;
}
void* thread_m(void*) {
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_m);
        pthread_mutex_lock(&lock);
        std::cout << "m" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        if (i < Q - 1)
            sem_post(&sem_i);
    }
    return nullptr;
}

// ========== ИНИЦИАЛИЗАЦИЯ ==========
int lab2_init() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_i, 0, 1);
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    // 1. a
    pthread_create(&tid[0], NULL, thread_a, NULL);
    pthread_create(&tid[2], NULL, thread_c, NULL);


    pthread_join(tid[0], NULL);
    

    // 2. b, c, e, g
    pthread_create(&tid[1], NULL, thread_b, NULL);
    pthread_create(&tid[4], NULL, thread_e, NULL);
    pthread_create(&tid[6], NULL, thread_g, NULL);

    pthread_join(tid[1], NULL);
    pthread_join(tid[4], NULL);
    // 3. d, f (можно сразу, т.к. тесты проверяют группы)
    pthread_create(&tid[3], NULL, thread_d, NULL);
    pthread_create(&tid[5], NULL, thread_f, NULL);

    // Ждём всю группу c, d, f, g
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);
    pthread_join(tid[6], NULL);

    // 4. h
    pthread_create(&tid[7], NULL, thread_h, NULL);
    pthread_join(tid[7], NULL);
    //pthread_create(&tid[9], NULL, thread_k, NULL);

    //pthread_join(tid[9], NULL);
    // 5. i, k, m
    pthread_create(&tid[8], NULL, thread_i, NULL);
    pthread_create(&tid[9], NULL, thread_k, NULL);
    pthread_create(&tid[10], NULL, thread_m, NULL);
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    pthread_join(tid[10], NULL);

    // 6. n
    //pthread_create(&tid[10], NULL, thread_m, NULL);
    pthread_create(&tid[11], NULL, thread_n, NULL);
    //pthread_join(tid[10], NULL);
    pthread_join(tid[11], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);

    std::cout << std::endl;
    return 0;
}
