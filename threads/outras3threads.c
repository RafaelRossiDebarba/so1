#include <pthread.h>
#include <stdio.h>

void *cria_thread(void *qualquercoisa);
void *cria_outra(void *qualquercoisa);

int main(){
    pthread_t tid1, tid2, tid3;
    char *st1= "!!!!!!! Oi Mundo !!!!!!!";
    char *st2= "*** Passo Fundo Tche ***";

    printf("Oi, sou o main ...\n");
    pthread_create(&tid1, NULL, cria_thread, st1);
    pthread_create(&tid2, NULL, cria_outra, NULL);
    pthread_create(&tid3, NULL, cria_thread, st2);
    printf("Apos criação das threads %ld, %ld e %ld\n", (long) tid1, (long) tid2, (long) tid3);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    printf("Agora ja sincronizei as threads e fim do programa\n");

    return (0);
}

void *cria_thread(void *qualquercoisa){
    printf("Sou a thread criada\n");
    printf("%s\n", (char *) qualquercoisa);
    printf("Final do processamento da thread %ld\n", (long) pthread_self());
    return NULL;
}

void *cria_outra(void *qualquercoisa){
    printf("CRIA_OUTRA --> Sou a thread criada\n");
    printf("Final do processamento da thread gerada por CRIA_OUTRA\n");

    return NULL;
}
