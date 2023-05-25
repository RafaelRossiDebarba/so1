#include <pthread.h>
#include <stdio.h>

void *cria_thread(void *qualquercoisa);

int main(){
    pthread_t tid1, tid2;
    int x=8, y=13;

    printf("Oi, sou o main ...\n");
    pthread_create(&tid1, NULL, cria_thread, &x);
    printf("Apos criação thread %ld\n", (long) tid1);
    pthread_create(&tid2, NULL, cria_thread, &y);
	printf("Apos criação thread %ld\n", (long) tid2);

    pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("Sincronizei as threads \n");
    return (0);
}

void *cria_thread(void *qualquercoisa){
	int *arg1, arg2;
	arg1 = (int *) qualquercoisa;
	arg2 = *arg1;
    printf("Sou a thread criada, com argumento %d\n", arg2);
    printf("Ou posso pegar o valor do argumento assim %d\n", *arg1);
    return NULL;
}

