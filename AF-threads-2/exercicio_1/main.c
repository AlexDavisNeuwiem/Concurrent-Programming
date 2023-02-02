#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//                 (main)      
//                    |
//    +----------+----+------------+
//    |          |                 |   
// worker_1   worker_2   ....   worker_n


// ~~~ argumentos (argc, argv) ~~~
// ./program n_threads

// ~~~ printfs  ~~~
// main thread (após término das threads filhas): "Contador: %d\n"
// main thread (após término das threads filhas): "Esperado: %d\n"

// Obs:
// - pai deve criar n_threads (argv[1]) worker threads 
// - cada thread deve incrementar contador_global (operador ++) n_loops vezes
// - pai deve esperar pelas worker threads  antes de imprimir!


int contador_global = 0;

void * incrementar(void *arg) {  // Função "incrementar" que incrementa a variável global "contador_global", todas as threads acessarão essa função
    
    int loop = *((int*) arg);

    for(int k = 0; k < loop; k++) {
        contador_global++;  // REGIÃO CRÍTICA: Aqui todas as threads modificam a varável global, esse trecho é responsável pela diferença entre "Contador" e "Esperado"
    }                      // Para ser mais específico, o real responsável por essa diferença é a mudança de prioridade do processo, que é causada pelo SO
    
    pthread_exit(NULL);  // Nenhum valor retornado
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);
    
    pthread_t th[n_threads];
    
    for (int i = 0; i< n_threads; i++ ) {
        pthread_create(&th[i], NULL, incrementar, (void*)&n_loops);  // Aqui são criadas n_threads que acessarão a função "incrementar"
    }
    
    for (int j = 0; j < n_threads; j++) {
        pthread_join(th[j], NULL);  // Aqui, a thread principal espera todas as n_threads terminarem de executar
    }
    
    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    return 0;
}
