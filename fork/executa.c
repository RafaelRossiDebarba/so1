#include <unistd.h>
#include <stdio.h>

/* Deve estar compilado o programa getpid.c com o
   binário executável (getpid) no mesmo diretorio */

int main(){
    char *progname="./getpid";
    char *argv[] = {progname, NULL};
    char *envp[] = {NULL};

    printf("Sou um processo executando com pid ...\n");
    printf("Tenho um SEGREDO para lhe contar.\n");
    printf("A resposta da prova é ...\n");
    execve(progname, argv, envp);
    sleep(1);
    printf("Pronto, contei ...\n");

    return(0);
}
