void* thread_i(void*) {
    for (int i = 0; i < 3; ++i) {
        sem_wait(&sem_i);
        pthread_mutex_lock(&lock);
        std::cout << 'i' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_k); // запускает k
    }
    return nullptr;
}

void* thread_k(void*) {
    for (int i = 0; i < 6; ++i) {
        sem_wait(&sem_k);
        pthread_mutex_lock(&lock);
        std::cout << 'k' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        sem_post(&sem_m); // запускает m
    }
    return nullptr;
}

void* thread_m(void*) {
    for (int i = 0; i < 6; ++i) {
        sem_wait(&sem_m);
        pthread_mutex_lock(&lock);
        std::cout << 'm' << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        // после первых 3 – продолжает вызывать k
        if (i >= 2) sem_post(&sem_k);
        else sem_post(&sem_i); // первые 3 круга замыкаются через i
    }
    return nullptr;
}
