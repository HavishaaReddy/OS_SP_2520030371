#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
pid_t pid = fork();

if(pid == 0) {
int out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
int err = open("error.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if(out < 0 || err < 0) {
perror("open");
exit(1);
}
dup2(out, STDOUT_FILENO);
dup2(err, STDERR_FILENO);
close(out);
close(err);
execlp("ls", "ls", "nonexistent_file", NULL);
perror("ls");
exit(1);
}

wait(NULL);
printf("Output written to output.txt\n");
printf("Error written to error.txt\n");
return 0;
}
