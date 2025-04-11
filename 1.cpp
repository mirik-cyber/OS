pthread_create(&tid[2], NULL, thread_c, NULL);
pthread_create(&tid[3], NULL, thread_d, NULL);
pthread_create(&tid[5], NULL, thread_f, NULL);
pthread_create(&tid[6], NULL, thread_g, NULL);
// все 4 потока работают параллельно!
pthread_join(tid[2], NULL);
pthread_join(tid[3], NULL);
pthread_join(tid[5], NULL);
pthread_join(tid[6], NULL);
