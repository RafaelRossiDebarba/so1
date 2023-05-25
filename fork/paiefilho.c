#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv){    
    pid_t pid;

    if ((pid = fork()) < 0)    {
        printf("Erro na criação do fork\n");
        exit(1);
    }
    if (pid == 0){
        //O código aqui dentro será executado no processo filho
        printf("Pid do Filho: %d\n", getpid());
    }
    else{
        //O código neste trecho será executado no processo pai
        printf("Pid do Pai: %d e pid do filho %d\n", getpid(), pid);
    }

    printf("Esta regiao sera executada por par e filho (ambos processos)\n\n");
    exit(0);
}