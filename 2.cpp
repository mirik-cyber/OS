int lab2_init() {
    pthread_mutex_init(&lock, nullptr);
    sem_init(&sem_i, 0, 1);
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    // ===== ИНТЕРВАЛ 1: a, c =====
    pthread_create(&tid[0], nullptr, thread_a, nullptr); // a
    pthread_create(&tid[2], nullptr, thread_c, nullptr); // c
    pthread_join(tid[0], nullptr); // ждем завершения a

    // ===== ИНТЕРВАЛ 2: b, e, g (c всё ещё может работать) =====
    pthread_create(&tid[1], nullptr, thread_b, nullptr); // b
    pthread_create(&tid[4], nullptr, thread_e, nullptr); // e
    pthread_create(&tid[6], nullptr, thread_g, nullptr); // g

    pthread_join(tid[1], nullptr); // b
    pthread_join(tid[4], nullptr); // e

    // ===== ИНТЕРВАЛ 3: d, f, (c и g уже запущены) =====
    pthread_create(&tid[3], nullptr, thread_d, nullptr); // d
    pthread_create(&tid[5], nullptr, thread_f, nullptr); // f

    pthread_join(tid[3], nullptr); // d
    pthread_join(tid[5], nullptr); // f
    pthread_join(tid[2], nullptr); // c
    pthread_join(tid[6], nullptr); // g

    // ===== ИНТЕРВАЛ 4: h, k =====
    pthread_create(&tid[7], nullptr, thread_h, nullptr); // h
    pthread_create(&tid[9], nullptr, thread_k, nullptr); // k
    pthread_join(tid[7], nullptr); // h

    // ===== ИНТЕРВАЛ 5: i, m (k уже запущен) =====
    pthread_create(&tid[8], nullptr, thread_i, nullptr); // i
    pthread_create(&tid[10], nullptr, thread_m, nullptr); // m

    pthread_join(tid[8], nullptr); // i
    pthread_join(tid[9], nullptr); // k
    pthread_join(tid[10], nullptr); // m

    // ===== ИНТЕРВАЛ 6: n =====
    pthread_create(&tid[11], nullptr, thread_n, nullptr); // n
    pthread_join(tid[11], nullptr);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);

    return 0;
}
