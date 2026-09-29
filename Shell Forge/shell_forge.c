#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 100

int main() {
    char input[MAX_INPUT];

    printf("====================================\n");
    printf("       Welcome to Shell Forge!\n");
    printf("====================================\n");

    while (1) {
        printf("shell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        char *args[MAX_ARGS];
        int count = 0;

        char *token = strtok(input, " ");

        while (token != NULL && count < MAX_ARGS - 1) {
            args[count++] = token;
            token = strtok(NULL, " ");
        }

        args[count] = NULL;

        if (strcmp(args[0], "exit") == 0)
            break;

        pid_t pid = fork();

        if (pid == 0) {
            execvp(args[0], args);
            perror("Command failed");
            exit(1);
        }
        else if (pid > 0) {
            waitpid(pid, NULL, 0);
        }
        else {
            perror("fork failed");
        }
    }

    printf("Goodbye from Shell Forge!\n");

    return 0;
}
