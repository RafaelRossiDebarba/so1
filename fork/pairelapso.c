#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main (int argc, char **argv){
    int pid;
    char *myargs [] = { NULL };
    char *myenv [] = { NULL };

    printf ("PAI Relapso --> Hello, World!\n");

    pid = fork ();

    if (pid < 0){
    	printf("Erro na criação do fork\n");
        exit(1);
    }
    if (pid == 0) { // Processo filho
        execve("./filho", myargs, myenv);
    }

    printf ("PAI Relapso --> Vai esperar o filho completar?\n");
    sleep(1);
    printf ("PAI Relapso --> Esperei 1 segundo (fui programado para esperar)\n");    
    printf ("PAI Relapso --> Finalizei, tchau!\n");    
    return (0);
}