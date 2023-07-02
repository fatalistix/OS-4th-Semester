#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int main(int argc, char * argv[])
{
    printf("<==> 1ST DIM: PID = %d\n", getpid());
    printf("1ST DIM: PROGRAM NAME = %s\n", argv[0]);
    sleep(10);

    pid_t childPid = fork();
    if (childPid == -1)
    {
        char error[256];
        sprintf(error, "%s: error calling fork child", argv[0]);
        perror(error);
        return errno;
    }

    if (childPid == 0)
    {
        printf("2ND DIM: PID = %d; PPID = %d\n", getpid(), getppid());
        pid_t grandChildPid = fork();
        if (grandChildPid == -1)
        {
            char error[256];
            sprintf(error, "%s: error calling fork grandchild", argv[0]);
            perror(error);
            return errno;
        }

        if (grandChildPid == 0)
        {
            while (1)
            {
                printf("3RD DIM: PRINTING PID = %d; PPID = %d\n", getpid(), getppid());
                sleep(2);
            }    
        }
        else
        {
            printf("2ND DIM: SLEEP FOR 10 SECONDS\n");
            sleep(10);
            printf("2ND DIM: RETURN 0");
            return 0;
        }
    }
    else
    {
        while (1)
        {
            printf("1ST DIM: PID = %d\n", getpid());
            sleep(2);
        }
    }

    return 0;
}
