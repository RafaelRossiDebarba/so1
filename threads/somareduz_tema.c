#include <pthread.h>
#include <stdio.h>
#include <unistd.h> //usleep
#include <stdlib.h>
#include <string.h>

void *soma(void *qualquercoisa);
void *dimi(void *qualquercoisa);

int global_x = 100; // Variavel global, compartilhada entre as threads

int main(int argc, char *argv[]){
    if(argc != 2) {
        printf("Verifique os parametros inseridos\nEx: ./somareduz 5\n");
        exit(1);
    }
    pthread_t tid1, tid2;
    int errors = 0, loop;
    loop = strtol(argv[1], NULL, 10);
    printf("Começando a execução\n");

    for(int i = 0; i < loop; i++) {
        pthread_create(&tid2, NULL, dimi, NULL);
        pthread_create(&tid1, NULL, soma, NULL);

        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        if(global_x != 100) {
            global_x = 100;
            errors ++;
        }
    }

    printf("Errors %d para %d execuções\n", errors, loop);
    return (0);
}

void *dimi(void *qualquercoisa){
    int i;
    
    i = global_x;
   // usleep(20); // Força uma perda da CPU
    i -= 50;
    global_x = i;

    return NULL;
}


void * soma(void *qualquercoisa){
    int i;

    i = global_x;
    //usleep(20);
    i += 50;
    global_x = i;

    return NULL;
}
