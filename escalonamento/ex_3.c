#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

int main () {
  int val1, val2;
  int witch = PRIO_PROCESS;
  id_t pid;
  pid = getpid();

  val1 = getpriority(witch, pid);
  nice(10);
  val2 = getpriority(witch, pid);

  printf("Inicial = %d\nFinal = %d\n", val1, val2);
}
