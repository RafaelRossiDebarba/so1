#include <stdio.h>
#include <unistd.h>

int main () {
  int val1, val2, original;

  original = nice(0);

  val1 = nice(5);

  val2 = nice(-5);

  printf("Valor inicial: %d\nValor elevado: %d\nValor subtraido %d\n", original, val1, val2);
}
