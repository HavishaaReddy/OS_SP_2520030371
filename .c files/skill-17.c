#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
pid_t jobs[3];
printf("Starting background jobs...\n");
for (int i = 0; i < 3; i++) {
jobs[i] = fork();
if (jobs[i] == 0) {
printf("Job %d running, PID = %d\n", i + 1, getpid());
sleep(5);
exit(0);
}
}
printf("\nActive Jobs:\n");
for (int i = 0; i < 3; i++) {
printf("[%d] PID = %d - Running\n", i + 1, jobs[i]);
}
for (int i = 0; i < 3; i++)
waitpid(jobs[i], NULL, 0);
printf("\nAll jobs completed.\n");
return 0;
}
