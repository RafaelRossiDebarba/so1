#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
  
void forkexample2(); 

int main(int argc, char **argv){ 
    forkexample2();
    return 0; 
} 

void forkexample2(){    
    char c = 'a';
    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("Erro na criação do fork\n");
        exit(1);
    }
    if (pid == 0){
        printf("FILHO --> Filho vai dormir 1 segundo\n"); 
        sleep(1);
        printf("\nFILHO --> Caractere vale [%c] e Endereco: %p\n", c, &c);
    }
    else {        
        c++; //É a mesma coisa de fazer c='b';
        printf("\n PAI  --> Caractere vale [%c] e Endereco: %p\n", c, &c);
    }
}