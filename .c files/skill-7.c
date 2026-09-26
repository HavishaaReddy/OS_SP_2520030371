#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
pid_t pid;
int status;

for(int i=0; i<3; i++) {
pid = fork();
if(pid == 0) {
printf("Child %d started, PID=%d\n", i+1, getpid());
sleep(2+i);
exit(i);
}
}

for(int i=0; i<3; i++) {
pid_t cpid = waitpid(-1, &status, 0);
if(WIFEXITED(status)) {
printf("Child PID=%d exited normally with status %d\n", cpid, WEXITSTATUS(status));
} else if(WIFSIGNALED(status)) {
printf("Child PID=%d terminated by signal %d\n", cpid, WTERMSIG(status));
}
}

printf("All children completed.\n");
return 0;
}
