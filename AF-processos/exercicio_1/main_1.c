#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {
    
    pid_t pid;      // Declaração da variável que armazena o PID de cada filho criado
    int quant = 2; // Quantidade de filhos que serão criados pelo Processo Principal
    
    for (int i = 0; i < quant; i++) {
            pid = fork();
            
            if (pid >= 0) { // Se pid >= 0, então deu certo (processo criado)
                if (pid == 0) {
                    printf("Processo filho %d criado\n", getpid()); // Apenas o processo filho entra aqui (PID igual a 0)
                    break; // Impede que o filho crie mais filhos pelo fork()
                } else {
                    printf("Processo pai criou %d\n", pid); // Apenas o processo pai entra aqui (PID maior que 0)
                    wait(NULL);
                }
            } else {
                printf("Deu errado\n");
                return 1;
            }

    }
    if (pid > 0 && wait(NULL) == -1) {
        printf("Processo pai finalizado!\n");
        return 0;
    } 
}
