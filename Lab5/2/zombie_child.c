#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char * argv[]) 
{
    printf("<==> PARENT: PID = %d <==>\n", getpid());
    printf("PARENT: PROGRAM NAME: %s\n", argv[0]);
    sleep(15);
    pid_t childPid = fork();
    if (childPid == -1)
    {
        char error[256];
        sprintf(error, "%s: error calling fork", argv[0]);
        perror(error);
        return errno;
    }
    if (childPid == 0)
    {
        printf("CHILD: PID = %d; PPID = %d\n", getpid(), getppid());
        sleep(10);
        printf("CHILD: RETURN WITH EXIT CODE 0\n");
        return 0;
    }
    else
    {
        printf("PARENT: SLEEP FOR 30 SECONDS\n");
        sleep(30);
        printf("PARENT: SLEEP EXITED\n");
    }
    return 0;
}
