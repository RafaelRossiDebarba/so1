#include<unistd.h> 
#include<stdio.h> 
#include<time.h> 
#include<signal.h> 

void mensagem() { 
    time_t tp; 
    time(&tp); 
    printf("%ld\n", (long) tp); 
} 

int main(int argc, char **argv) { 
    signal(SIGALRM, mensagem); 
    printf("*** inicio do programa\n"); 
    while (1){ 
        alarm(5); 
        pause(); 
        printf("Executando\n");
    } 
}