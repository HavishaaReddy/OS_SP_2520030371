#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
pid_t pid = fork();

if(pid == 0) {
int fd = open("input.txt", O_RDONLY);
if(fd < 0) {
perror("open");
exit(1);
}
dup2(fd, STDIN_FILENO);
close(fd);
execlp("wc", "wc", "-l", NULL);
perror("execlp");
exit(1);
}

wait(NULL);
return 0;
}
