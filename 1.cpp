#include "lab2.h"
#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <map>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>

#define NUM_THREADS 12
#define Q 3

pthread_t tid[NUM_THREADS];
pthread_mutex_t lock;
sem_t sem_i, sem_k, sem_m, sem_a, sem_c;

std::map<std::string, bool> flags;
std::map<std::string, std::mutex> mutexes;
std::map<std::string, std::condition_variable> conds;

unsigned int lab2_thread_graph_id() { return 5; }
const char* lab2_unsynchronized_threads() { return "cdfg"; }
const char* lab2_sequential_threads() { return "ikm"; }

void wait_flag(const std::string& name) {
    std::unique_lock<std::mutex> lk(mutexes[name]);
    conds[name].wait(lk, [&]{ return flags[name]; });
}

void set_flag(const std::string& name) {
    std::lock_guard<std::mutex> lk(mutexes[name]);
    flags[name] = true;
    conds[name].notify_all();
}

// Потоки

void* thread_a(void*) {
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_a);
        pthread_mutex_lock(&lock);
        std::cout << 'a' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_c);
    }
    set_flag("a");
    return nullptr;
}

void* thread_b(void*) {
    wait_flag("a");
    wait_flag("c1");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'b' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("b");
    return nullptr;
}

void* thread_c(void*) {
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_c);
        pthread_mutex_lock(&lock);
        std::cout << 'c' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_a);
    }
    wait_flag("a");
    set_flag("c1");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'c' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    wait_flag("b");
    wait_flag("e");
    set_flag("c2");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'c' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("c");
    return nullptr;
}

void* thread_d(void*) {
    wait_flag("b");
    wait_flag("e");
    wait_flag("c2");
    wait_flag("g1");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'd' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("d");
    return nullptr;
}

void* thread_e(void*) {
    wait_flag("a");
    wait_flag("c1");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'e' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("e");
    return nullptr;
}

void* thread_f(void*) {
    wait_flag("b");
    wait_flag("e");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'f' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("f");
    return nullptr;
}

void* thread_g(void*) {
    wait_flag("a");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    wait_flag("b");
    wait_flag("e");
    wait_flag("c1");
    set_flag("g1");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    wait_flag("f");
    wait_flag("c");
    wait_flag("d");
    set_flag("g2");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'g' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("g");
    return nullptr;
}

void* thread_h(void*) {
    wait_flag("f");
    wait_flag("c");
    wait_flag("d");
    wait_flag("g2");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'h' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    set_flag("h");
    return nullptr;
}

void* thread_i(void*) {
    wait_flag("h");
    wait_flag("g");
    wait_flag("k1");
    for (int i = 0; i < Q; ++i) {
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
    wait_flag("f");
    wait_flag("c");
    wait_flag("d");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'k' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    wait_flag("h");
    wait_flag("g");
    set_flag("k1");
    for (int i = 0; i < Q; ++i) {
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
    wait_flag("h");
    wait_flag("g");
    wait_flag("k1");
    for (int i = 0; i < Q; ++i) {
        sem_wait(&sem_m);
        pthread_mutex_lock(&lock);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_i);
    }
    set_flag("m");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

void* thread_n(void*) {
    wait_flag("m");
    for (int i = 0; i < Q; ++i) {
        pthread_mutex_lock(&lock);
        std::cout << 'n' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
    }
    return nullptr;
}

// Инициализация

int lab2_init() {
    pthread_mutex_init(&lock, nullptr);
    sem_init(&sem_i, 0, 1);
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);
    sem_init(&sem_a, 0, 1);
    sem_init(&sem_c, 0, 0);

    std::vector<std::string> flag_names = {
        "a", "b", "c", "d", "e", "f", "g", "h", "m", "c1", "c2", "g1", "g2", "k1"
    };
    for (const auto& name : flag_names) {
        flags[name] = false;
    }

    pthread_create(&tid[0], nullptr, thread_a, nullptr);
    pthread_create(&tid[2], nullptr, thread_c, nullptr);
    pthread_join(tid[0], nullptr);

    pthread_create(&tid[1], nullptr, thread_b, nullptr);
    pthread_create(&tid[4], nullptr, thread_e, nullptr);
    pthread_create(&tid[6], nullptr, thread_g, nullptr);
    pthread_join(tid[1], nullptr);
    pthread_join(tid[4], nullptr);

    pthread_create(&tid[3], nullptr, thread_d, nullptr);
    pthread_create(&tid[5], nullptr, thread_f, nullptr);
    pthread_join(tid[2], nullptr);
    pthread_join(tid[3], nullptr);
    pthread_join(tid[5], nullptr);

    pthread_create(&tid[7], nullptr, thread_h, nullptr);
    pthread_create(&tid[9], nullptr, thread_k, nullptr);
    pthread_join(tid[7], nullptr);
    pthread_join(tid[6], nullptr);

    pthread_create(&tid[8], nullptr, thread_i, nullptr);
    pthread_create(&tid[10], nullptr, thread_m, nullptr);
    pthread_join(tid[8], nullptr);
    pthread_join(tid[9], nullptr);

    pthread_create(&tid[11], nullptr, thread_n, nullptr);
    pthread_join(tid[10], nullptr);
    pthread_join(tid[11], nullptr);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_a);
    sem_destroy(&sem_c);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);
    return 0;
}
