// ===== ИНТЕРВАЛ 1: только a =====
pthread_create(&tid[0], nullptr, thread_a, nullptr); // a
pthread_join(tid[0], nullptr); // ждем завершения a

// ===== ИНТЕРВАЛ 2: теперь можно c, b, e, g =====
pthread_create(&tid[2], nullptr, thread_c, nullptr); // c
pthread_create(&tid[1], nullptr, thread_b, nullptr); // b
pthread_create(&tid[4], nullptr, thread_e, nullptr); // e
pthread_create(&tid[6], nullptr, thread_g, nullptr); // g
