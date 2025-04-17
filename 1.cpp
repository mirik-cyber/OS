void* thread_m(void*) {
    for (int i = 0; i < 2*Q; ++i) {
        if (i < Q - 1)
            sem_wait(&sem_m);
        pthread_mutex_lock(&lock);
        std::cout << "m" << std::flush;
        pthread_mutex_unlock(&lock);
        computation();
        if (i < Q - 1)
            sem_post(&sem_i);
    }
    sem_post(&sem_i);
    return nullptr;
}

accaaccaabegcgeecbggecgbebbgcccdfgfcdfcgdcgcgdffcgdgggghkkkkkhhhhikmikmikmikmikmmmmmmnnnnn
