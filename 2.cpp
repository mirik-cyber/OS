#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

#define Q 5
#define MULT 3

pthread_t tid[12];
pthread_mutex_t lock;
sem_t sem_i, sem_k, sem_m;

unsigned int lab2_thread_graph_id() { return 5; }
const char* lab2_unsynchronized_threads() { return "cdfg"; }
const char* lab2_sequential_threads() { return "ikm"; }

void print(char c, int count) {
    for (int i = 0; i < count; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << c << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
}

// ======== НЕСИНХРОНИЗИРОВАННЫЕ ========
void* thread_c(void*) { print('c', MULT * 3); return nullptr; }
void* thread_d(void*) { print('d', MULT * 1); return nullptr; }
void* thread_f(void*) { print('f', MULT * 1); return nullptr; }
void* thread_g(void*) { print('g', MULT * 3); return nullptr; }

// ======== СИНХРОНИЗИРОВАННЫЕ ========
void* thread_i(void*) {
    for (int i = 0; i < MULT; ++i) {
        sem_wait(&sem_i);
        pthread_mutex_lock(&lock);
        std::cout << 'i' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_k);
    }
    return nullptr;
}

void* thread_k(void*) {
    for (int i = 0; i < MULT * 2; ++i) {
        sem_wait(&sem_k);
        pthread_mutex_lock(&lock);
        std::cout << 'k' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_m);
    }
    return nullptr;
}

void* thread_m(void*) {
    for (int i = 0; i < MULT * 2; ++i) {
        sem_wait(&sem_m);
        pthread_mutex_lock(&lock);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_i);
    }
    return nullptr;
}

// ======== ПРОЧИЕ ========
void* thread_a(void*) { print('a', MULT * 1); return nullptr; }
void* thread_b(void*) { print('b', MULT * 1); return nullptr; }
void* thread_e(void*) { print('e', MULT * 1); return nullptr; }
void* thread_h(void*) { print('h', MULT * 1); return nullptr; }
void* thread_n(void*) { print('n', MULT * 1); return nullptr; }

// ======== ИНИЦИАЛИЗАЦИЯ ========
int lab2_init() {
    pthread_mutex_init(&lock, nullptr);
    sem_init(&sem_i, 0, 1);
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    pthread_create(&tid[0], nullptr, thread_a, nullptr);
    pthread_create(&tid[1], nullptr, thread_b, nullptr);
    pthread_create(&tid[2], nullptr, thread_c, nullptr);
    pthread_create(&tid[3], nullptr, thread_d, nullptr);
    pthread_create(&tid[4], nullptr, thread_e, nullptr);
    pthread_create(&tid[5], nullptr, thread_f, nullptr);
    pthread_create(&tid[6], nullptr, thread_g, nullptr);
    pthread_create(&tid[7], nullptr, thread_h, nullptr);
    pthread_create(&tid[8], nullptr, thread_i, nullptr);
    pthread_create(&tid[9], nullptr, thread_k, nullptr);
    pthread_create(&tid[10], nullptr, thread_m, nullptr);
    pthread_create(&tid[11], nullptr, thread_n, nullptr);

    for (int i = 0; i < 12; ++i) {
        pthread_join(tid[i], nullptr);
    }

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);

    return 0;
}
