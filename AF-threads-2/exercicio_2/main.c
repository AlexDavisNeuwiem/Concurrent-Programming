#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

struct ptrs {  // struct com todos os ponteiros e variáveis importantes
    double* a;
    double* b;
    double* c;
    int com;  // Variável que determina o começo do intervalo que a thread trabalhará
    int fim;  // variável que determina o fim do intervalo que a thread trabalhará
};

void * soma_vetor(void *arg) {

    struct ptrs *str = (struct ptrs*) arg;  // Ponteiro para struct ptrs

    for (int i = str->com; i < str->fim; i++) {
        str->c[i] = str->a[i] + str->b[i];  // Aqui, o vetor "c" é alterado via ponteiro
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    struct ptrs str;  // Declaração da struct "str"
    str.a = a;
    str.b = b;
    str.c = c;

    if (n_threads > a_size) {
        n_threads = a_size;  // Para que haja apenas o número necessário de threads
    }

    int q = a_size / n_threads;  // Valor inteiro da divisão
    int r = a_size % n_threads;  // Resto da divisão

    pthread_t th[n_threads];

    for (int i = 0; i < n_threads; i++ ) {

        str.com = i*q;
        str.fim = (i + 1)*q;

        if (r != 0 && i == n_threads - 1) {  // A última thread fará o resto do vetor, caso não seja uma divisão inteira
            str.com = i*q;
            str.fim = (i + 1)*q + r;
        }

        pthread_create(&th[i], NULL, soma_vetor, (void*) &str);
    }

    for (int j = 0; j < n_threads; j++) {
        pthread_join(th[j], NULL);
    }

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);
    
    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
}
