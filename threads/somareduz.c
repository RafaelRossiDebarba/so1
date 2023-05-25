#include <pthread.h>
#include <stdio.h>
#include <unistd.h> //usleep

void *soma(void *qualquercoisa);
void *dimi(void *qualquercoisa);

int global_x = 100; // Variavel global, compartilhada entre as threads

int main(){
    pthread_t tid1, tid2;

    printf("Oi, sou o main ...\n");

    pthread_create(&tid2, NULL, dimi, NULL);
 //   usleep(10);
    pthread_create(&tid1, NULL, soma, NULL);

    printf("Apos criação das threads %ld e %ld\n", (long) tid1, (long) tid2);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("Valor final de global_x %d\n", global_x);
    return (0);   
}

void *dimi(void *qualquercoisa){    
    int i;
    
    printf("Diminuindo ...\n");

    i = global_x;
    //usleep(20); // Força uma perda da CPU
    i -= 50;
    global_x = i;       

    return NULL;
}


void * soma(void *qualquercoisa){
    int i;

    printf("Somando ...\n");

    i = global_x;
    i += 50;
    global_x = i;       

    return NULL;
}
