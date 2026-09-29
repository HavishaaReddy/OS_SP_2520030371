#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
int pipefd[2];
if (pipe(pipefd) == -1) {
perror("pipe");
return 1;
}
pid_t p1 = fork();
if (p1 == 0) {
int fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fd < 0) {
perror("open");
exit(1);
}
dup2(pipefd[1], STDOUT_FILENO);
close(pipefd[0]);
close(pipefd[1]);
execlp("ls", "ls", NULL);
perror("ls");
exit(1);
}
pid_t p2 = fork();
if (p2 == 0) {
int fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fd < 0) {
perror("open");
exit(1);
}
dup2(pipefd[0], STDIN_FILENO);
dup2(fd, STDOUT_FILENO);
close(fd);
close(pipefd[0]);
close(pipefd[1]);
execlp("wc", "wc", "-l", NULL);
perror("wc");
exit(1);
}
close(pipefd[0]);
close(pipefd[1]);
waitpid(p1, NULL, 0);
waitpid(p2, NULL, 0);
printf("Result stored in result.txt\n");
return 0;
}
