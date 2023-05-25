#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv) {
  printf("p1\n");
  printf("p2\n");
  pid_t pid;
  int status;

  pid = fork ();

  if(pid < 0) {
    printf("Error\n");
    exit(1);
  }
  if(pid == 0) {
    printf("p3\n");
  }
  else {
    printf("p4\n");
    waitpid(pid, &status, 0);
    printf("p5\n");
  }

  return (0);
}
