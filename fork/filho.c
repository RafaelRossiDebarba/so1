#include <stdio.h>
#include <time.h> 
#include <unistd.h>

#define A 501
#define B 613 
#define C 707
    
int main (int argc, char **argv){   
    int i, j;
    long sum;
    time_t tpi, tpf; 

    // Filho realiza algum tipo de processamento
    printf ("FILHO --> Hello, World, sou o processo %d!\n", getpid());

    time(&tpi);
    for (j = 0; j < 300; j++ ) { // apenas para simular tempo de processamento
        for (i =0; i < 5000000; i++) {
            sum = A * i + B * i * i + C;
            sum %= 543;
        }
        if (j % 50 == 0){
        	time(&tpf);
        	printf("FILHO --> Valor de j: %d transcorrido %lds\n", j, (long)tpf-tpi);
        }
    }

    printf ("FILHO --> Trabalho Completo!\n");
    printf ("FILHO --> Tchau!\n");

    return (0);
}