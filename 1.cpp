#include <pthread.h>
#include <semaphore.h>
#include <iostream>
#include <map>
#include <string>

pthread_mutex_t flag_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag_cond = PTHREAD_COND_INITIALIZER;
std::map<std::string, bool> flags;

void wait_flag(const std::string& name) {
    pthread_mutex_lock(&flag_mutex);
    while (!flags[name])
        pthread_cond_wait(&flag_cond, &flag_mutex);
    pthread_mutex_unlock(&flag_mutex);
}

void set_flag(const std::string& name) {
    pthread_mutex_lock(&flag_mutex);
    flags[name] = true;
    pthread_cond_broadcast(&flag_cond);
    pthread_mutex_unlock(&flag_mutex);
}
