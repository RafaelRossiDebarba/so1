#include <stdio.h>
#include <sched.h>

int main () {
  int min, max;
  min = sched_get_priority_min(SCHED_FIFO);
  max = sched_get_priority_max(SCHED_FIFO);

  printf("Valor minimo FIFO: %d\n", min);
  printf("Valor maximo FIFO: %d\n", max);

  min = sched_get_priority_min(SCHED_RR);
  max = sched_get_priority_max(SCHED_RR);

  printf("Valor minimo RR: %d\n", min);
  printf("Valor maximo RR: %d\n", max);
}
