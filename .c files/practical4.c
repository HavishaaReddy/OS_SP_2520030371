#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i;

    for (i = 0; i < 3; i++)
    {
        if (fork() == 0)
        {
            printf("Child %d PID=%d\n", i + 1, getpid());
            return 0;
        }
    }

    for (i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    printf("All children completed.\n");

    return 0;
}
