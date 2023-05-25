#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char **argv){
    int pid;
    char *myargs [] = { NULL };
    char *myenv [] = { NULL };
    int status;
    pid_t ret;

    printf ("PAI Zeloso  --> Hello, World!\n");

    pid = fork ();

    if (pid == 0) { // Processo Filho
        execve("./filho", myargs, myenv);
    }

    printf ("PAI Zeloso  --> Vai esperar o filho completar?\n");    
    ret = waitpid(pid, &status, 0);

    printf("PAI Zeloso  --> Acabou! o filho %d e eu também fui ...\n", (int) ret);
    return (0);
}