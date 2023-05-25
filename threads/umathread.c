#include <pthread.h>
#include <stdio.h>

void *cria_thread(void *qualquercoisa);

int main(){
    pthread_t tid;

    printf("Oi, sou o main ...\n");
    pthread_create(&tid, NULL, cria_thread, NULL);
    printf("Apos criação thread %ld\n", (long) tid);

    pthread_join(tid, NULL);
    printf("Ja sincronizei com a thread\n");
    return (0);
}

void *cria_thread(void *qualquercoisa){
    printf("Sou a thread criada\n");
    return NULL;
}

