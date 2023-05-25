#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

int main(){		
	long arq = syscall(SYS_open, "aula.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	syscall(SYS_write, arq, "Hello, World\n", 14);
	syscall(SYS_close, arq);
	syscall(SYS_exit, 0);
}
