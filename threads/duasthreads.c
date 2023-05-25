#include <pthread.h>
#include <stdio.h>

void *cria_thread(void *qualquercoisa);

int main(){
    pthread_t tid1, tid2;

    printf("Oi, sou o main ...\n");
    pthread_create(&tid1, NULL, cria_thread, NULL);
    printf("Apos criação thread %ld\n", (long) tid1);
    pthread_create(&tid2, NULL, cria_thread, NULL);
	printf("Apos criação thread %ld\n", (long) tid2);

    pthread_join(tid1, NULL);
    printf("Ja sincronizei com a primeira thread (%ld) criada\n", tid1);
	pthread_join(tid2, NULL);
	printf("Sincronizei a segunda thread criada (%ld)\n", tid2);
    return (0);
}

void *cria_thread(void *qualquercoisa){
    printf("Sou a thread criada\n");
    return NULL;
}

