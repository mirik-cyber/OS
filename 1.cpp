#include "lab2.h"
#include <cstring>
#include <semaphore.h>

#define NUMBER_OF_THREADS 11  // a,b,c,d,e,f,g,i,k,m,n

// Объявления функций
unsigned int lab2_thread_graph_id();
const char* lab2_unsynchronized_threads();
const char* lab2_sequential_threads();
int lab2_init();

// Глобальные переменные
pthread_t tid[NUMBER_OF_THREADS];
pthread_mutex_t lock;
sem_t sem_b, sem_c, sem_d, sem_g;

// Прототипы функций потоков
void* thread_a(void* ptr);
void* thread_b(void* ptr);
void* thread_c(void* ptr);
void* thread_d(void* ptr);
void* thread_e(void* ptr);
void* thread_f(void* ptr);
void* thread_g(void* ptr);
void* thread_i(void* ptr);
void* thread_k(void* ptr);
void* thread_m(void* ptr);
void* thread_n(void* ptr);

// Реализации функций
unsigned int lab2_thread_graph_id() {
    return 3;
}

const char* lab2_unsynchronized_threads() {
    return "dfgi";
}

const char* lab2_sequential_threads() {
    return "bcdg";
}

void* thread_a(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "a";
        pthread_mutex_unlock(&lock);
        computation();
    }
    
    pthread_create(&tid[1], NULL, thread_b, NULL);
    pthread_create(&tid[2], NULL, thread_c, NULL);
    pthread_create(&tid[3], NULL, thread_d, NULL);
    pthread_create(&tid[6], NULL, thread_g, NULL);
    
    for (int i = 1; i < NUMBER_OF_THREADS; ++i) {
        if (i != 6) pthread_join(tid[i], NULL);
    }
    pthread_join(tid[6], NULL);
    
    return ptr;
}

void* thread_b(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_b);
        pthread_mutex_lock(&lock);
        std::cout << "b";
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_c);
    }
    
    pthread_create(&tid[4], NULL, thread_e, NULL);
    pthread_join(tid[4], NULL);
    return ptr;
}

void* thread_c(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_c);
        pthread_mutex_lock(&lock);
        std::cout << "c";
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_d);
    }
    
    pthread_create(&tid[7], NULL, thread_i, NULL);
    pthread_create(&tid[5], NULL, thread_f, NULL);
    pthread_join(tid[7], NULL);
    pthread_join(tid[5], NULL);
    return ptr;
}

void* thread_d(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_d);
        pthread_mutex_lock(&lock);
        std::cout << "d";
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_g);
    }
    
    pthread_create(&tid[8], NULL, thread_k, NULL);
    pthread_create(&tid[9], NULL, thread_m, NULL);
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    return ptr;
}

void* thread_e(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "e";
        pthread_mutex_unlock(&lock);
        computation();
        sched_yield();
    }
    
    pthread_create(&tid[7], NULL, thread_i, NULL);
    pthread_create(&tid[5], NULL, thread_f, NULL);
    pthread_join(tid[7], NULL);
    pthread_join(tid[5], NULL);
    return ptr;
}

void* thread_f(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "f";
        pthread_mutex_unlock(&lock);
        computation();
        sched_yield();
    }
    
    pthread_create(&tid[8], NULL, thread_k, NULL);
    pthread_create(&tid[9], NULL, thread_m, NULL);
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    return ptr;
}

void* thread_g(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_g);
        pthread_mutex_lock(&lock);
        std::cout << "g";
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_b);
    }
    
    pthread_create(&tid[10], NULL, thread_n, NULL);
    pthread_join(tid[10], NULL);
    return ptr;
}


void* thread_i(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "i";
        pthread_mutex_unlock(&lock);
        computation();
        sched_yield();
    }
    
    pthread_create(&tid[10], NULL, thread_n, NULL);
    pthread_join(tid[10], NULL);
    return ptr;
}

void* thread_k(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "k";
        pthread_mutex_unlock(&lock);
        computation();
        sched_yield();
    }
    
    pthread_create(&tid[10], NULL, thread_n, NULL);
    pthread_join(tid[10], NULL);
    return ptr;
}

void* thread_m(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "m";
        pthread_mutex_unlock(&lock);
        computation();
        sched_yield();
    }
    return ptr;
}

void* thread_n(void* ptr) {
    for (int i = 0; i < 5; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << "n";
        pthread_mutex_unlock(&lock);
        computation();
        sched_yield();
    }
    return ptr;
}

int lab2_init() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_b, 0, 1);
    sem_init(&sem_c, 0, 0);
    sem_init(&sem_d, 0, 0);
    sem_init(&sem_g, 0, 0);

    pthread_create(&tid[0], NULL, thread_a, NULL);
    pthread_join(tid[0], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_b);
    sem_destroy(&sem_c);
    sem_destroy(&sem_d);
    sem_destroy(&sem_g);

    return 0;
}
