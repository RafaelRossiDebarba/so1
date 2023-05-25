#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char **argv){
	struct timeval tval_before, tval_after, tval_result;

	gettimeofday(&tval_before, NULL);
	// Aqui um código que se quer marcar o tempo
	usleep(7200); 
	//sleep(1);	
	
	gettimeofday(&tval_after, NULL);

	timersub(&tval_after, &tval_before, &tval_result);

	printf("Valor decimal em us. 1 us = 1/1000000 segundo\n");
	printf("Exemplo com sleep (s) e usleep (us)\n");
	printf("Inicio (tempo): %ld.%06ld\n", (long int)tval_before.tv_sec, (long int)tval_before.tv_usec);	
	printf("Atual (tempo) : %ld.%06ld\n", (long int)tval_after.tv_sec, (long int)tval_after.tv_usec);
	printf("Decorrido (R) : %ld.%06ld s\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	

	return 0;
}