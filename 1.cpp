#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

#define NUM_THREADS 12
#define Q 5

pthread_t tid[NUM_THREADS];
pthread_mutex_t lock;
pthread_mutex_t interval_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t interval_cond = PTHREAD_COND_INITIALIZER;
int global_interval = 1;

sem_t sem_i, sem_k, sem_m;

unsigned int lab2_thread_graph_id() { return 5; }
const char* lab2_unsynchronized_threads() { return "cdfg"; }
const char* lab2_sequential_threads() { return "ikm"; }

void wait_for_interval(int target) {
    pthread_mutex_lock(&interval_lock);
    while (global_interval != target)
        pthread_cond_wait(&interval_cond, &interval_lock);
    pthread_mutex_unlock(&interval_lock);
}

void next_interval() {
    pthread_mutex_lock(&interval_lock);
    ++global_interval;
    pthread_cond_broadcast(&interval_cond);
    pthread_mutex_unlock(&interval_lock);
}

void computation() {
    usleep(1000); // для тестов
}

// ===== Потоки =====
void* thread_a(void*) {
    wait_for_interval(1);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'a' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_b(void*) {
    wait_for_interval(2);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'b' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_c(void*) {
    wait_for_interval(1); for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'c' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    wait_for_interval(2); for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'c' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    wait_for_interval(3); for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'c' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_d(void*) {
    wait_for_interval(3);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'd' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_e(void*) {
    wait_for_interval(2);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'e' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_f(void*) {
    wait_for_interval(3);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'f' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_g(void*) {
    wait_for_interval(2); for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'g' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    wait_for_interval(3); for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'g' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    wait_for_interval(4); for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'g' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_h(void*) {
    wait_for_interval(4);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'h' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_k(void*) {
    wait_for_interval(4);
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_k);
        pthread_mutex_lock(&lock); std::cout << 'k' << std::flush; pthread_mutex_unlock(&lock); computation();
        sem_post(&sem_m);
    }
    return nullptr;
}
void* thread_i(void*) {
    wait_for_interval(5);
    sem_post(&sem_i); // старт для цикла
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_i);
        pthread_mutex_lock(&lock); std::cout << 'i' << std::flush; pthread_mutex_unlock(&lock); computation();
        sem_post(&sem_k);
    }
    return nullptr;
}
void* thread_m(void*) {
    wait_for_interval(5);
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_m);
        pthread_mutex_lock(&lock); std::cout << 'm' << std::flush; pthread_mutex_unlock(&lock); computation();
        sem_post(&sem_i);
    }
    wait_for_interval(6);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'm' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}
void* thread_n(void*) {
    wait_for_interval(6);
    for (int i = 0; i < Q; ++i) { pthread_mutex_lock(&lock); std::cout << 'n' << std::flush; pthread_mutex_unlock(&lock); computation(); }
    return nullptr;
}

// ===== lab2_init() =====
int lab2_init() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_i, 0, 0); // раньше было 1
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    pthread_create(&tid[0], NULL, thread_a, NULL);
    pthread_create(&tid[1], NULL, thread_b, NULL);
    pthread_create(&tid[2], NULL, thread_c, NULL);
    pthread_create(&tid[3], NULL, thread_d, NULL);
    pthread_create(&tid[4], NULL, thread_e, NULL);
    pthread_create(&tid[5], NULL, thread_f, NULL);
    pthread_create(&tid[6], NULL, thread_g, NULL);
    pthread_create(&tid[7], NULL, thread_h, NULL);
    pthread_create(&tid[8], NULL, thread_i, NULL);
    pthread_create(&tid[9], NULL, thread_k, NULL);
    pthread_create(&tid[10], NULL, thread_m, NULL);
    pthread_create(&tid[11], NULL, thread_n, NULL);

    sleep(1); next_interval(); // → 2
    sleep(1); next_interval(); // → 3
    sleep(1); next_interval(); // → 4
    sleep(1); next_interval(); // → 5

    // ⏸ подождать завершения i, k, m
    pthread_join(tid[8], NULL);  // i
    pthread_join(tid[9], NULL);  // k
    pthread_join(tid[10], NULL); // m

    next_interval();             // → 6

    pthread_join(tid[11], NULL); // n

    // Остальные
    pthread_join(tid[0], NULL); // a
    pthread_join(tid[1], NULL); // b
    pthread_join(tid[2], NULL); // c
    pthread_join(tid[3], NULL); // d
    pthread_join(tid[4], NULL); // e
    pthread_join(tid[5], NULL); // f
    pthread_join(tid[6], NULL); // g
    pthread_join(tid[7], NULL); // h

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);
    return 0;
}
