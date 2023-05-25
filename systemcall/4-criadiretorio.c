#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char **argv){	
	int s;

	printf("Criando diretorio testeaula em /tmp\n");
	s = mkdir("/tmp/testeaula", 0777);
	
	if (!s)
		printf("Diretorio criado\nConferir ...\n");
	else
		printf("Problema ao criar diretorio. Retorno %d\n",s);
	return(0);
}
