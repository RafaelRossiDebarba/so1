#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <string.h>

void arquivo(char arc[]) {
  int fd = open(arc, O_RDONLY | O_CREAT);
  int sz;
  char *c = (char *) calloc(100, sizeof(char));
  
  if(fd == -1) {
    printf("Arquivo não pode ser aberto\n");
    return;
  }
  printf("------------- Conteudo arquivo:\n");
  while(1) {
    sz = read(fd, c, 100);
    if(sz == 0) {
      break;
    }
    c[sz] = '\0';

    write(1, c, 100);

  }
  printf("\n----------------Fim do Conteudo\n");

  if(close(fd) < 0) {
    printf("Não foi possivel fechar o arquivo\n");
  }
}


int main (int argc, char *argv[]) {
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  int isArq = 0;
  struct stat sb;
  char arq_name[100];

  if(argc != 2) {
    printf("Verifique os pametros enviados.\nÉ permitido o envio de somente 1 argumento\n");
    exit(EXIT_FAILURE);
  }
  if(stat(argv[1], &sb) == -1) {
    printf("Arquivo não encontrado\n");
    exit(EXIT_FAILURE);
  }

  strcat(arq_name, argv[1]);
  printf("Nome arquivo %s\n", arq_name);

  int a = access(argv[1], X_OK);
  if(a == -1) {
    printf("Não pode executar\n");
  }
  else {
    printf("Pode executar\n");
  }
  a = access(argv[1], R_OK);
  if(a == -1) {
    printf("Não pode ler\n");
  }
  else {
    printf("Pode ler\n");
  }
  a = access(argv[1], W_OK);
  if(a == -1) {
    printf("Não pode escrever\n");
  }
  else {
    printf("Pode escrever\n");
  }
  
  printf("Tipo do arquivo: ");
  switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  printf("block device\n");                break;
    case S_IFCHR:  printf("character device\n");            break;
    case S_IFDIR:  printf("Diretorio\n");                   break;
    case S_IFIFO:  printf("FIFO/pipe\n");                   break;
    case S_IFLNK:  printf("symlink\n");                     break;
    case S_IFREG:  printf("texto\n");   isArq = 1;          break;
    case S_IFSOCK: printf("socket\n");                      break;
    default:       printf("unknown?\n");                    break;
  }
  printf("ID %ld\n", (long) sb.st_uid);
  printf("ID grupo: %ld\n", (long) sb.st_gid);
  printf("Tamanho arquivo: %lld bytes\n", (long long) sb.st_size);
  printf("Ultima modificação: %s", ctime(&sb.st_mtime));
  printf("Ultimo acesso: %s", ctime(&sb.st_atime));
  if(isArq == 1) {
    arquivo(argv[1]);
  }
  printf("PID: %d\n", getpid());
  getrusage(RUSAGE_SELF, &usage);
  printf("CPU modo usuario: %li,%li\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
  printf("CPU modo sistema: %li,%li\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
  printf("Memoria usada: %ld\n", usage.ru_maxrss);
  printf("Memoria dados usados: %ld\n", usage.ru_idrss);
  printf("Memoria pilha usada: %ld\n", usage.ru_isrss);
}
