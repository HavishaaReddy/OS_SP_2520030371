/*
==========================================================
                    CO MAPPING
============================================================

CO1:
    - User mode / Kernel mode
    - System call interface
    - Linux system calls
    - Shell / Terminal
    - getpid()
    - getppid()
    - read()
    - write()
    - strace can be used externally

CO2:
    - fork()
    - exit()
    - _exit()
    - waitpid()
    - sigaction()
    - SIGCHLD
    - kill()
    - WIFEXITED()
    - WEXITSTATUS()
    - WIFSIGNALED()
    - WTERMSIG()

CO3:
    - Anonymous Pipe
    - Signal-based communication
    - SIGCHLD
============================================================
*/


/* =========================================================
   HEADER FILES
   ========================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         // CO1: Linux/POSIX system calls
#include <sys/types.h>
#include <sys/wait.h>       // CO2: waitpid(), WIFEXITED(), etc.
#include <signal.h>         // CO2/CO3: signals
#include <string.h>
#include <errno.h>


#define MAX_CHILDREN 50


/* =========================================================
   STRUCTURE
   ========================================================= */

typedef struct
{
    pid_t pid;

    /*
     * Stores the termination option selected by the user.
     */
    int option;

} ChildInfo;


/* =========================================================
   CO2 / CO3 - SIGCHLD
   =========================================================

   SIGCHLD is generated for the parent when a child
   terminates.

   CO2:
       Process management / SIGCHLD

   CO3:
       Signal-based communication
   ========================================================= */

volatile sig_atomic_t child_finished = 0;


void sigchld_handler(int sig)
{
    /*
     * CO3 - Signal handling
     */

    (void)sig;

    child_finished = 1;
}


/* =========================================================
   TERMINATION OPTION NAMES
   ========================================================= */

const char *get_option_name(int option)
{
    switch (option)
    {
        case 1:
            return "Normal Exit";

        case 2:
            return "SIGTERM";

        case 3:
            return "SIGKILL";

        case 4:
            return "SIGINT";

        case 5:
            return "SIGABRT";

        case 6:
            return "SIGQUIT";

        default:
            return "Unknown";
    }
}


/* =========================================================
   DISPLAY TERMINATION OPTIONS
   ========================================================= */

void display_options()
{
    printf("\n");

    printf("============================================\n");
    printf("       SELECT TERMINATION METHOD\n");
    printf("============================================\n");

    printf("1. Normal Exit (exit())\n");
    printf("2. SIGTERM\n");
    printf("3. SIGKILL\n");
    printf("4. SIGINT\n");
    printf("5. SIGABRT\n");
    printf("6. SIGQUIT\n");

    printf("============================================\n");
}


/* =========================================================
   CHILD PROCESS
   ========================================================= */

void child_process(int child_number,
                   int read_fd)
{
    int option;


    /* =====================================================
       CO1 - SYSTEM CALLS

       getpid()  -> gets current process ID
       getppid() -> gets parent process ID

       These are Linux/POSIX system calls.
       ===================================================== */

    printf("\n[START] Child %d started\n",
           child_number);

    printf("        PID  : %d\n",
           getpid());

    printf("        PPID : %d\n",
           getppid());

    fflush(stdout);


    /* =====================================================
       CO3 - ANONYMOUS PIPE

       The parent sends the selected termination option
       to the child through a pipe.

       read() is a system call used to receive data.
       ===================================================== */

    if (read(read_fd,
             &option,
             sizeof(option)) != sizeof(option))
    {
        perror("read");

        close(read_fd);

        /*
         * CO2 - _exit()
         */
        _exit(1);
    }

    close(read_fd);


    printf("[Child %d] Selected method: %s\n",
           child_number,
           get_option_name(option));

    fflush(stdout);

    sleep(1);


    /* =====================================================
       CO2 - PROCESS TERMINATION
       ===================================================== */

    switch (option)
    {

        /* =================================================
           OPTION 1
           NORMAL TERMINATION

           CO2:
               exit()
               ================================================= */

        case 1:

            printf("[Child %d] Calling exit(0)...\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 - exit()
             *
             * Normal process termination.
             */
            exit(0);


        /* =================================================
           OPTION 2
           SIGTERM

           CO2:
               Signals
               kill()
           ================================================= */

        case 2:

            printf("[Child %d] Sending SIGTERM to itself...\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 / CO3 - Signal
             *
             * kill() sends SIGTERM.
             */
            kill(getpid(), SIGTERM);

            /*
             * CO2 - _exit()
             *
             * Normally this line will not execute.
             */
            _exit(1);


        /* =================================================
           OPTION 3
           SIGKILL

           CO2:
               Signals
               kill()
           ================================================= */

        case 3:

            printf("[Child %d] Sending SIGKILL to itself...\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 / CO3 - Signal
             *
             * SIGKILL immediately terminates the process.
             */
            kill(getpid(), SIGKILL);

            _exit(1);


        /* =================================================
           OPTION 4
           SIGINT

           CO2:
               Signals
               kill()
           ================================================= */

        case 4:

            printf("[Child %d] Sending SIGINT to itself...\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 / CO3 - Signal
             */
            kill(getpid(), SIGINT);

            _exit(1);


        /* =================================================
           OPTION 5
           SIGABRT

           CO2:
               abort()
               Signal termination
           ================================================= */

        case 5:

            printf("[Child %d] Calling abort()...\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 / CO3
             *
             * abort() generates SIGABRT.
             */
            abort();

            _exit(1);


        /* =================================================
           OPTION 6
           SIGQUIT

           CO2:
               Signals
               kill()
           ================================================= */

        case 6:

            printf("[Child %d] Sending SIGQUIT to itself...\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 / CO3 - Signal
             */
            kill(getpid(), SIGQUIT);

            _exit(1);


        /* =================================================
           INVALID OPTION
           ================================================= */

        default:

            printf("[Child %d] Option not found.\n",
                   child_number);

            fflush(stdout);

            /*
             * CO2 - _exit()
             */
            _exit(2);
    }
}


/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main()
{
    int number_of_children;

    ChildInfo children[MAX_CHILDREN];


    /* =====================================================
       CO1 - USER MODE / SYSTEM CALL

       The program is a user-space application.

       getpid() requests process information from Linux.

       Terminal/Shell
             ↓
       User Program
             ↓
       System Call
             ↓
       Linux Kernel
             ↓
       Result
       ===================================================== */

    printf("\n");

    printf("====================================================\n");

    printf("          PROCESS TERMINATION MESSAGES\n");

    printf("====================================================\n");


    /*
     * CO1 - getpid()
     */

    printf("Parent PID: %d\n",
           getpid());


    /* =====================================================
       INPUT
       ===================================================== */

    printf("\nEnter the number of child processes: ");

    scanf("%d",
          &number_of_children);


    /* =====================================================
       INPUT VALIDATION
       ===================================================== */

    if (number_of_children <= 0 ||
        number_of_children > MAX_CHILDREN)
    {
        printf("\nInvalid number of child processes.\n");

        printf("Enter a number between 1 and %d.\n",
               MAX_CHILDREN);

        return 1;
    }


    /* =====================================================
       CO2 - SIGCHLD

       sigaction() installs the signal handler.

       When a child terminates, Linux sends SIGCHLD
       to the parent.
       ===================================================== */

    struct sigaction sa;

    memset(&sa,
           0,
           sizeof(sa));


    /*
     * CO2 / CO3 - Signal handler
     */

    sa.sa_handler = sigchld_handler;


    /*
     * CO2 / CO3 - Signal mask
     */

    sigemptyset(&sa.sa_mask);


    sa.sa_flags = SA_RESTART;


    /*
     * CO2 / CO3 - sigaction()
     */

    if (sigaction(SIGCHLD,
                   &sa,
                   NULL) == -1)
    {
        perror("sigaction");

        return 1;
    }


    /* =====================================================
       CREATE CHILDREN ONE BY ONE

       IMPORTANT FLOW:

       START
         ↓
       PID
         ↓
       INPUT
         ↓
       TERMINATE
         ↓
       WAIT
         ↓
       DETECT
         ↓
       REPORT
         ↓
       NEXT CHILD
       ===================================================== */

    for (int i = 0;
         i < number_of_children;
         i++)
    {

        int option;

        /*
         * CO3 - Anonymous Pipe
         *
         * Parent → Child communication.
         */

        int pipefd[2];


        /*
         * CO3 - pipe()
         *
         * Creates anonymous pipe.
         */

        if (pipe(pipefd) == -1)
        {
            perror("pipe");

            return 1;
        }


        /* =================================================
           CO2 - fork()

           Creates a child process.
           ================================================= */

        pid_t pid = fork();


        if (pid < 0)
        {
            perror("fork");

            return 1;
        }


        /* =================================================
           CHILD PROCESS
           ================================================= */

        if (pid == 0)
        {

            /*
             * CO3 - Close unused pipe write end.
             */

            close(pipefd[1]);


            /*
             * Display START and PID first.
             */

            child_process(i + 1,
                          pipefd[0]);


            /*
             * CO2 - _exit()
             */

            _exit(1);
        }


        /* =================================================
           PARENT PROCESS
           ================================================= */

        /*
         * CO3 - Close unused pipe read end.
         */

        close(pipefd[0]);


        /*
         * Store child's PID.
         */

        children[i].pid = pid;


        /*
         * Give child time to display START/PID.
         */

        usleep(100000);


        /* =================================================
           INPUT FOR THIS CHILD
           ================================================= */

        display_options();


        printf("Enter option for Child %d: ",
               i + 1);


        scanf("%d",
              &option);


        /* =================================================
           INVALID OPTION HANDLING
           ================================================= */

        while (option < 1 ||
               option > 6)
        {

            printf("Option not found.\n");


            printf("Enter option for Child %d again: ",
                   i + 1);


            scanf("%d",
                  &option);
        }


        /*
         * Store selected option.
         */

        children[i].option = option;


        /* =================================================
           CO3 - ANONYMOUS PIPE

           Send selected termination option from parent
           to child.
           ================================================= */

        write(pipefd[1],
              &option,
              sizeof(option));


        close(pipefd[1]);


        /* =================================================
           CO2 - waitpid()

           Parent waits for THIS child.

           The parent will NOT create the next child until
           the current child has terminated and its result
           has been detected.

           This keeps the output clean.
           ================================================= */

        int status;

        pid_t result;


        do
        {

            result = waitpid(pid,
                             &status,
                             0);

        }

        while (result == -1 &&
               errno == EINTR);


        /* =================================================
           TERMINATION REPORT
           ================================================= */

        printf("\n");

        printf("--------------------------------------------\n");

        printf("        TERMINATION REPORT - CHILD %d\n",
               i + 1);

        printf("--------------------------------------------\n");


        printf("PID         : %d\n",
               pid);


        printf("Selected    : %s\n",
               get_option_name(option));


        /* =================================================
           CO2 - WIFEXITED()

           Checks whether child terminated normally.
           ================================================= */

        if (WIFEXITED(status))
        {

            printf("Exit Reason : Normal termination\n");


            /*
             * CO2 - WEXITSTATUS()
             *
             * Gets child's exit status.
             */

            printf("Exit Status : %d\n",
                   WEXITSTATUS(status));
        }


        /* =================================================
           CO2 - WIFSIGNALED()

           Checks whether child was terminated by signal.
           ================================================= */

        else if (WIFSIGNALED(status))
        {

            /*
             * CO2 - WTERMSIG()
             *
             * Gets the signal which terminated the child.
             */

            int sig = WTERMSIG(status);


            printf("Exit Reason : Signal termination\n");


            printf("Signal No.  : %d\n",
                   sig);


            /*
             * CO1 / Linux system information
             *
             * strsignal() converts signal number to name.
             */

            printf("Signal Name : %s\n",
                   strsignal(sig));


            /* ---------------------------------------------
               Identify exact termination reason
               --------------------------------------------- */

            if (sig == SIGTERM)
            {
                printf("Reason      : Terminated by SIGTERM\n");
            }

            else if (sig == SIGKILL)
            {
                printf("Reason      : Terminated by SIGKILL\n");
            }

            else if (sig == SIGINT)
            {
                printf("Reason      : Terminated by SIGINT\n");
            }

            else if (sig == SIGABRT)
            {
                printf("Reason      : Terminated by SIGABRT\n");
            }

            else if (sig == SIGQUIT)
            {
                printf("Reason      : Terminated by SIGQUIT\n");
            }
        }


        else
        {
            printf("Exit Reason : Unknown termination state\n");
        }


        printf("--------------------------------------------\n");
    }


    /* =====================================================
       FINAL OUTPUT
       ===================================================== */

    printf("\n");

    printf("====================================================\n");

    printf("       ALL CHILD PROCESSES COMPLETED\n");

    printf("====================================================\n");


    /*
     * CO1 - getpid()
     */

    printf("Parent PID: %d\n",
           getpid());


    printf("\nProcess Termination Monitoring Completed.\n");


    return 0;
}
