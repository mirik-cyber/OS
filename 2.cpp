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

void computation() {
    // Заглушка для вычислений
}

// ========== ПОТОКИ ==========
void* thread_a(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'a' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_b(void*) {
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
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'c' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    pthread_join(tid[1], NULL);
    pthread_join(tid[4], NULL);
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
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_h(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'h' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_n(void*) {
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'n' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

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
        std::cout << 'k' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        if (i < Q - 1)
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
        sem_post(&sem_i);
    }
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

// ========== ИНИЦИАЛИЗАЦИЯ ==========
int lab2_init() {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        std::cerr << "Mutex init failed" << std::endl;
        return 1;
    }
    if (sem_init(&sem_i, 0, 1) != 0) {
        std::cerr << "Semaphore i init failed" << std::endl;
        return 1;
    }
    if (sem_init(&sem_k, 0, 0) != 0) {
        std::cerr << "Semaphore k init failed" << std::endl;
        return 1;
    }
    if (sem_init(&sem_m, 0, 0) != 0) {
        std::cerr << "Semaphore m init failed" << std::endl;
        return 1;
    }

    // 1. a
    if (pthread_create(&tid[0], NULL, thread_a, NULL) != 0) {
        std::cerr << "Thread a creation failed" << std::endl;
        return 1;
    }
    if (pthread_create(&tid[2], NULL, thread_c, NULL) != 0) {
        std::cerr << "Thread c creation failed" << std::endl;
        return 1;
    }

    pthread_join(tid[0], NULL);

    // 2. b, e, g
    if (pthread_create(&tid[1], NULL, thread_b, NULL) != 0) {
        std::cerr << "Thread b creation failed" << std::endl;
        return 1;
    }
    if (pthread_create(&tid[4], NULL, thread_e, NULL) != 0) {
        std::cerr << "Thread e creation failed" << std::endl;
        return 1;
    }
    if (pthread_create(&tid[6], NULL, thread_g, NULL) != 0) {
        std::cerr << "Thread g creation failed" << std::endl;
        return 1;
    }

    pthread_join(tid[1], NULL);
    pthread_join(tid[4], NULL);

    // 3. d, f
    if (pthread_create(&tid[3], NULL, thread_d, NULL) != 0) {
        std::cerr << "Thread d creation failed" << std::endl;
        return 1;
    }
    if (pthread_create(&tid[5], NULL, thread_f, NULL) != 0) {
        std::cerr << "Thread f creation failed" << std::endl;
        return 1;
    }

    // Ждём всю группу c, d, f, g
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);

    // 4. h
    if (pthread_create(&tid[7], NULL, thread_h, NULL) != 0) {
        std::cerr << "Thread h creation failed" << std::endl;
        return 1;
    }
    if (pthread_create(&tid[9], NULL, thread_k, NULL) != 0) {
        std::cerr << "Thread k creation failed" << std::endl;
        return 1;
    }
    pthread_join(tid[7], NULL);
    pthread_join(tid[6], NULL);

    // 5. i, m
    if (pthread_create(&tid[8], NULL, thread_i, NULL) != 0) {
        std::cerr << "Thread i creation failed" << std::endl;
        return 1;
    }
    if (pthread_create(&tid[10], NULL, thread_m, NULL) != 0) {
        std::cerr << "Thread m creation failed" << std::endl;
        return 1;
    }
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);

    // 6. n
    if (pthread_create(&tid[11], NULL, thread_n, NULL) != 0) {
        std::cerr << "Thread n creation failed" << std::endl;
        return 1;
    }
    pthread_join(tid[10], NULL);
    pthread_join(tid[11], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);

    return 0;
}
