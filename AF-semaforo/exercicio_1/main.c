#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

FILE* out;

sem_t esc_a, esc_b;
//sem_t lock_a, lock_b;

void *thread_a(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
        sem_wait(&esc_b);
        //sem_wait(&lock_a);
        
        fprintf(out, "A");
        fflush(stdout);

        //sem_post(&lock_a);
        sem_post(&esc_a);
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {
        sem_wait(&esc_a);
        //sem_wait(&lock_b);
        
        fprintf(out, "B");
        fflush(stdout);

        //sem_post(&lock_b);
        sem_post(&esc_b);

    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }
    int iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    pthread_t ta, tb;

    sem_init(&esc_a, 0, 1);
    sem_init(&esc_b, 0, 0);
    //sem_init(&lock_a, 0, 1);
    //sem_init(&lock_b, 0, 1);

    // Cria threads
    pthread_create(&ta, NULL, thread_a, &iters);
    pthread_create(&tb, NULL, thread_b, &iters);

    // Espera pelas threads
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    sem_destroy(&esc_a);
    sem_destroy(&esc_b);
    //sem_destroy(&lock_a);
    //sem_destroy(&lock_b);

    //Imprime quebra de linha e fecha arquivo
    fprintf(out, "\n");
    fclose(out);
  
    return 0;
}
