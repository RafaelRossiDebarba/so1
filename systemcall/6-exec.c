#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *msg = "\n\nHello, World!\n\n";
    char *progname="/bin/ls";
    char *argv[] = {progname, "-l", "-a", NULL};
    char *envp[] = {NULL};

    write(1, msg, strlen(msg));
	  execve(progname, argv, envp);
	
	exit(0);
}

