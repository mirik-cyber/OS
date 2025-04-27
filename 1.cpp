int lab2_init() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_i, 0, 1);
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);
    sem_init(&sem_a, 0, 1);
    sem_init(&sem_b, 0, 0);
    sem_init(&sem_e, 0, 0);
    sem_init(&sem_h, 0, 0);
    sem_init(&sem_n, 0, 0);

    // 1. a и c параллельно
    pthread_create(&tid[0], NULL, thread_a, NULL);
    pthread_create(&tid[2], NULL, thread_c, NULL);

    // НЕ ЖДЁМ А! Только параллельный запуск.

    // 2. b, e, g (после начала a, c)
    pthread_create(&tid[1], NULL, thread_b, NULL);
    pthread_create(&tid[4], NULL, thread_e, NULL);
    pthread_create(&tid[6], NULL, thread_g, NULL);

    // 3. d, f параллельно
    pthread_create(&tid[3], NULL, thread_d, NULL);
    pthread_create(&tid[5], NULL, thread_f, NULL);

    // Ждём ВСЕ: c, b, e, g, d, f
    pthread_join(tid[2], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[4], NULL);
    pthread_join(tid[6], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);
    pthread_join(tid[0], NULL); // Теперь можно догнать "a"!

    // 4. h
    pthread_create(&tid[7], NULL, thread_h, NULL);
    pthread_join(tid[7], NULL);

    // 5. i, k, m
    pthread_create(&tid[8], NULL, thread_i, NULL);
    pthread_create(&tid[9], NULL, thread_k, NULL);
    pthread_create(&tid[10], NULL, thread_m, NULL);
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    pthread_join(tid[10], NULL);

    // 6. n
    pthread_create(&tid[11], NULL, thread_n, NULL);
    pthread_join(tid[11], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);
    sem_destroy(&sem_a);
    sem_destroy(&sem_b);
    sem_destroy(&sem_e);
    sem_destroy(&sem_h);
    sem_destroy(&sem_n);

    return 0;
}
