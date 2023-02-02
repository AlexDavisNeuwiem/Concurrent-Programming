#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

int main(int argc, char** argv) {
    
    pid_t Fpid;  // Declaração da variável que armazena o PID de cada filho criado
    pid_t Npid;  // Declaração da variável que armazena o PID de cada neto criado
    int fil = 2; // Quantidade de filhos que serão criados pelo Processo Principal
    int net = 3; // Quantidade de netos que serão criados pelo Processo Principal
    
    // O "for" abaixo é relativo aos filhos, ou seja, é o laço de repetição que gerará cada um dos futuros pais
    
    for (int i = 0; i < fil; i++) {
            Fpid = fork();
            
            if (Fpid >= 0) {
                if (Fpid == 0) {
                    printf("Processo %d, filho de %d\n", getpid(), getppid()); // Apenas o processo filho entra aqui (Fpid igual a 0)
                    break; // Impede que o filho crie mais filhos pelo fork()
                } else {
                    wait(NULL);
                }
            } else {
                printf("Deu errado\n");
                return 1;
            }

    }
    
    // O "for" abaixo é relativo aos netos, de forma a gerar cada um deles
    
    if (Fpid == 0) {
        for (int j = 0; j < net; j++) {
            Npid = fork();
            
            if (Npid >= 0) {
                if (Npid == 0) {
                    printf("Processo %d, filho de %d\n", getpid(), getppid()); // Apenas o processo neto entra aqui (Npid igual a 0)
                    sleep(5);
                    break; // Impede que o neto crie mais filhos pelo fork()
                } else {
                    wait(NULL);
                    printf( "Processo %d finalizado\n", Npid);
                }
            } else {
                printf("Deu errado\n");
                return 1;
            }
        }
    }
    
    if (Fpid == 0 && Npid > 0 && wait(NULL) == -1) {
        printf( "Processo %d finalizado\n", getpid());
    }
    
    if (Fpid > 0 && wait(NULL) == -1) {
        printf("Processo principal %d finalizado\n", getpid());
        return 0;
    } 
}

