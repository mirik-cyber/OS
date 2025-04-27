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
    pthread_create(&tid[0], NULL, thread_a, NULL); // a
    pthread_create(&tid[2], NULL, thread_c, NULL); // c

    pthread_join(tid[0], NULL); // Ждём ТОЛЬКО a!

    // 2. b, e, g (после a, ПАРАЛЛЕЛЬНО с c)
    pthread_create(&tid[1], NULL, thread_b, NULL); // b
    pthread_create(&tid[4], NULL, thread_e, NULL); // e
    pthread_create(&tid[6], NULL, thread_g, NULL); // g

    // Теперь ждём ВСЮ группу: c, b, e, g
    pthread_join(tid[2], NULL); // c
    pthread_join(tid[1], NULL); // b
    pthread_join(tid[4], NULL); // e
    pthread_join(tid[6], NULL); // g

    // 3. d, f параллельно (после завершения группы c,b,e,g)
    pthread_create(&tid[3], NULL, thread_d, NULL); // d
    pthread_create(&tid[5], NULL, thread_f, NULL); // f

    pthread_join(tid[3], NULL); // d
    pthread_join(tid[5], NULL); // f

    // 4. h (после g)
    pthread_create(&tid[7], NULL, thread_h, NULL);
    pthread_join(tid[7], NULL);

    // 5. i, k, m (после h)
    pthread_create(&tid[8], NULL, thread_i, NULL);
    pthread_create(&tid[9], NULL, thread_k, NULL);
    pthread_create(&tid[10], NULL, thread_m, NULL);
    pthread_join(tid[8], NULL);
    pthread_join(tid[9], NULL);
    pthread_join(tid[10], NULL);

    // 6. n (после m)
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

