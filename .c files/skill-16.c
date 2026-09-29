#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
pid_t pid = fork();
if (pid < 0) {
perror("fork");
return 1;
}
if (pid == 0) {
printf("Background process started. PID = %d\n", getpid());
sleep(5);
printf("Background process completed.\n");
exit(0);
}
printf("Parent continues immediately.\n");
printf("Background PID = %d\n", pid);
printf("Parent is not waiting immediately.\n");
sleep(2);
waitpid(pid, NULL, 0);
printf("Background job finished.\n");
return 0;
}
