#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <unistd.h>
#include "lab2.h"

#define MULTIPLIER 4

pthread_mutex_t output_mutex;

sem_t sem_i, sem_k, sem_m;

const int ITERATIONS = MULTIPLIER * 3; // базовое количество итераций на каждом участке

void computation(); // прототип (если требуется)

void* thread_func_unsync(void* arg) {
    char c = *(char*)arg;
    for (int i = 0; i < ITERATIONS; ++i) {
        pthread_mutex_lock(&output_mutex);
        std::cout << c << std::flush;
        pthread_mutex_unlock(&output_mutex);
        computation();
    }
    return nullptr;
}

void* thread_func_i(void* arg) {
    for (int i = 0; i < ITERATIONS; ++i) {
        sem_wait(&sem_i);

        pthread_mutex_lock(&output_mutex);
        std::cout << 'i' << std::flush;
        pthread_mutex_unlock(&output_mutex);

        computation();
        sem_post(&sem_k);
    }
    return nullptr;
}

void* thread_func_k(void* arg) {
    for (int i = 0; i < ITERATIONS; ++i) {
        sem_wait(&sem_k);

        pthread_mutex_lock(&output_mutex);
        std::cout << 'k' << std::flush;
        pthread_mutex_unlock(&output_mutex);

        computation();
        sem_post(&sem_m);
    }
    return nullptr;
}

void* thread_func_m(void* arg) {
    for (int i = 0; i < ITERATIONS; ++i) {
        sem_wait(&sem_m);

        pthread_mutex_lock(&output_mutex);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&output_mutex);

        computation();
        sem_post(&sem_i);
    }
    return nullptr;
}

void* thread_func_n(void* arg) {
    // Просто обычный поток без строгой синхронизации
    for (int i = 0; i < ITERATIONS; ++i) {
        pthread_mutex_lock(&output_mutex);
        std::cout << 'n' << std::flush;
        pthread_mutex_unlock(&output_mutex);
        computation();
    }
    return nullptr;
}

// Возвращает номер графа
unsigned int lab2_thread_graph_id() {
    return 5;
}

// Потоки без синхронизации
const char* lab2_unsynchronized_threads() {
    return "cdfg";
}

// Потоки с синхронизацией семафорами
const char* lab2_sequential_threads() {
    return "ikm";
}

// Главная функция
int lab2_init() {
    pthread_t threads[8]; // c, d, f, g, i, k, m, n
    char thread_names[] = {'c', 'd', 'f', 'g'};

    pthread_mutex_init(&output_mutex, nullptr);
    sem_init(&sem_i, 0, 1); // i начинает
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    // Создаем несинхронизированные потоки: c, d, f, g
    for (int i = 0; i < 4; ++i) {
        pthread_create(&threads[i], nullptr, thread_func_unsync, &thread_names[i]);
    }

    // Создаем синхронизированные потоки: i, k, m
    pthread_create(&threads[4], nullptr, thread_func_i, nullptr);
    pthread_create(&threads[5], nullptr, thread_func_k, nullptr);
    pthread_create(&threads[6], nullptr, thread_func_m, nullptr);

    // Ждем завершения потоков несинхронизированных
    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], nullptr);
    }

    // Ждем завершения синхронизированных потоков
    pthread_cancel(threads[4]); // останавливаем вечные циклы корректно
    pthread_cancel(threads[5]);
    pthread_cancel(threads[6]);

    pthread_join(threads[4], nullptr);
    pthread_join(threads[5], nullptr);
    pthread_join(threads[6], nullptr);

    // После окончания работы m — запускаем n
    pthread_create(&threads[7], nullptr, thread_func_n, nullptr);
    pthread_join(threads[7], nullptr);

    pthread_mutex_destroy(&output_mutex);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);

    return 0;
}
