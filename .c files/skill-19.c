#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
if (sig == SIGINT)
printf("\nSIGINT received!\n");
}

int main() {
signal(SIGINT, handle_signal);
printf("Program running...\n");
printf("Press Ctrl+C to send SIGINT.\n");
while (1) {
printf("Waiting for signal...\n");
sleep(2);
}
return 0;
}
