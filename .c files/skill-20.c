#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int sig) {
printf("\nSIGINT caught by process.\n");
printf("Cleaning resources...\n");
printf("Program exiting safely.\n");
exit(0);
}

int main() {
signal(SIGINT, sigint_handler);
printf("Process PID: %d\n", getpid());
printf("Press Ctrl+C to terminate safely.\n");
while (1) {
printf("Running...\n");
sleep(2);
}
return 0;
}
