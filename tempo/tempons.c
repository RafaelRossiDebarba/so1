#include <stdio.h>
#include <time.h>

#define BILHAO 1000000000

void timespecAdd(struct timespec t1, struct timespec t2, struct timespec *res);
void timespecSub(struct timespec t1, struct timespec t2, struct timespec *res);

int main(int argc, char *argv[]) {
    struct timespec ts, tsadd, tsresult, tsactual;    

    tsadd.tv_sec = 0;
    tsadd.tv_nsec = 12000; // 12 us

    clock_gettime(CLOCK_MONOTONIC, &ts);    
    nanosleep(&tsadd, NULL);   
    clock_gettime(CLOCK_MONOTONIC, &tsactual);
    
    printf("Valor decimal em ns. 1 ns = 1/1000000000 segundo\n");
    printf("Exemplo usando nanosleep\n");
    printf("Inicio (tempo): %ld.%09ld\n", (long int)ts.tv_sec, (long int)ts.tv_nsec);    
    timespecSub(tsactual, ts, &tsresult);
    printf("Decorrido (A) : %ld.%09ld s\n", (long int)tsresult.tv_sec, (long int)tsresult.tv_nsec);        

    return 0;
}

/* res = t1 + t2 */
void timespecAdd(struct timespec t1, struct timespec t2, struct timespec *res){
    long sec = t1.tv_sec + t2.tv_sec;
    long nsec = t1.tv_nsec + t2.tv_nsec;
    if (nsec >= BILHAO) {
        nsec -= BILHAO;
        sec++;
    }
    res->tv_sec = sec;
    res->tv_nsec = nsec;    
}

/* res = t1 - t2 */
void timespecSub(struct timespec t1, struct timespec t2, struct timespec *res){
    long sec = t1.tv_sec - t2.tv_sec;
    long nsec = t1.tv_nsec - t2.tv_nsec;
    if (nsec < 0) {
        nsec += BILHAO;
        sec --;
    }
    res->tv_sec = sec;
    res->tv_nsec = nsec;    
}

