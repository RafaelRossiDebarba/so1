#include <pthread.h>
#include <stdio.h>

void *quadrado(void *qualquercoisa);

int retorno; // Note que essa variavel é global

int main(){
    pthread_t tid;
    int x=8;    
    int *ret[1];

    printf("Oi, sou o main ...\n");
    pthread_create(&tid, NULL, quadrado, &x);
    printf("Apos criação thread %ld\n", (long) tid);

    pthread_join(tid,(void **)&(ret[0]));
	printf("Sincronizei e recebi %d\n", *ret[0] );
	printf("Ou uso a variavel global %d\n", retorno );
    return (0);
}

void *quadrado(void *qualquercoisa){
	int arg;
	arg = *((int *) qualquercoisa);

    retorno = (arg * arg);	
    printf("Recebi o argumento %d e vou retornar seu quadrado = %d\n", arg, retorno);

    pthread_exit (&retorno);
    return NULL;
}

