#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <cstring>

#define NUM_THREADS 12
#define Q 3

pthread_t tid[NUM_THREADS];
pthread_mutex_t lock;
sem_t sem_i, sem_k, sem_m;
int err;

unsigned int lab2_thread_graph_id() {
    return 5;
}

const char* lab2_unsynchronized_threads() {
    return "cdfg";
}

const char* lab2_sequential_threads() {
    return "ikm";
}

// Функции-потоки
void* thread_a(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "a" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_b(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "b" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_c(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "c" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_d(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "d" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_e(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "e" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_f(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "f" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_g(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "g" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_h(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "h" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

// Синхронизированные потоки
void* thread_i(void* ptr) {
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

void* thread_k(void* ptr) {
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

void* thread_m(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_m);
        pthread_mutex_lock(&lock);
        std::cout << "m" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        if (i < Q - 1) sem_post(&sem_i);  // запустить следующий цикл
    }
    return nullptr;
}

void* thread_n(void* ptr) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "n" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

int lab2_init() {
    // Мьютекс
    if (pthread_mutex_init(&lock, NULL) != 0) {
        std::cerr << "Mutex init failed\n";
        return 1;
    }

    // Семафоры
    sem_init(&sem_i, 0, 1);  // первый поток активен
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    // Запуск потоков
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

    // Ожидание завершения
    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(tid[i], nullptr);

    // Очистка
    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);
    std::cout << std::endl;
    return 0;
}
