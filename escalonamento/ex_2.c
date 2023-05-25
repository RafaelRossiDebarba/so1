#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


int main () {
  int priority;
  int witch = PRIO_PROCESS;
  id_t pid = getpid();

  priority = getpriority(witch, pid);

  printf("Prioridade: %d\n", priority);
}
