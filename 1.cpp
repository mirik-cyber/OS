int lab2_init() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&sem_i, 0, 1); // i активен первым
    sem_init(&sem_k, 0, 0);
    sem_init(&sem_m, 0, 0);

    // 1. a
    pthread_create(&tid[0], NULL, thread_a, NULL);
    pthread_join(tid[0], NULL); // ждем завершения

    // 2. b, c, e, g
    pthread_create(&tid[1], NULL, thread_b, NULL);
    pthread_create(&tid[2], NULL, thread_c, NULL);
    pthread_create(&tid[4], NULL, thread_e, NULL);
    pthread_create(&tid[6], NULL, thread_g, NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

    // 3. d (после b и c)
    pthread_create(&tid[3], NULL, thread_d, NULL);
    pthread_join(tid[4], NULL);
    pthread_join(tid[6], NULL);

    // 4. f (после e и g)
    pthread_create(&tid[5], NULL, thread_f, NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[5], NULL);

    // 5. h (после d и f)
    pthread_create(&tid[7], NULL, thread_h, NULL);
    pthread_join(tid[7], NULL);

    // 6. i, k, m — синхронные
    pthread_create(&tid[8], NULL, thread_i, NULL);
    pthread_create(&tid[9], NULL, thread_k, NULL);
    pthread_create(&tid[10], NULL, thread_m, NULL);
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    pthread_join(tid[10], NULL);

    // 7. n
    pthread_create(&tid[11], NULL, thread_n, NULL);
    pthread_join(tid[11], NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&sem_i);
    sem_destroy(&sem_k);
    sem_destroy(&sem_m);
    std::cout << std::endl;
    return 0;
}
