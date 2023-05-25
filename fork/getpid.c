#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("\nGETPID --> Pid do processo: %d\n", (int) getpid());
    return (0);
}