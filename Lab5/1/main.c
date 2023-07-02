#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int myGlobalVar = 4;


int main(int argc, char * argv[])
{
    int myLocalVar = 15;


    printf("<==> PARENT PID = %d <==>\n", getpid());
    printf("<===================>\n");
    printf("PARENT: &myGlobalVar = %p; myGlobalVar = %d\n", &myGlobalVar, myGlobalVar);
    printf("PARENT: &myLocalVar  = %p; myLocalVar  = %d\n", &myLocalVar,  myLocalVar); 
    printf("<===================>\n");


    int childPid = fork();
    if (childPid == -1)
    {
        char error[256];
        sprintf(error, "%s: error creating a new process", argv[0]);
        perror(error);
        return errno;
    }
    if (childPid == 0) 
    {
        printf("CHILD: Hello from child:\nCHILD: PID = %d\nCHILD: PPID = %d\n", getpid(), getppid());
        printf("<===================>\n");
        printf("CHILD: Printing addresses and values of variables\n");
        printf("CHILD: &myGlobalVar = %p; myGlobalVar = %d\n", &myGlobalVar, myGlobalVar);
        printf("CHILD: &myLocalVar =  %p; myLocalVar  = %d\n", &myLocalVar,  myLocalVar); 
        printf("<===================>\n");
        printf("CHILD: Changing myGlobalVar to 6 and myLocalVar to 5\n");
        myGlobalVar = 6;
        myLocalVar  = 5; 
        printf("<===================>\n");
        printf("CHILD: myGlobalVar = %d\nCHILD: myLocalVar = %d\n", myGlobalVar, myLocalVar); 
        printf("<===================>\n");
        sleep(25);
        exit(5);
    }
    else 
    {
        sleep(3);
        printf("PARENT: myGlobalVar = %d\nPARENT: myLocalVar = %d\n", myGlobalVar, myLocalVar);
        sleep(30);

        int status;
        if (wait(&status) == -1)
        {
            char error[256];
            sprintf(error, "%s: error waiting for a child", argv[0]);
            perror(error);
            return errno;
        }
        
        if (WIFEXITED(status))
        {
            printf("PARENT: exited with status %d\n", WEXITSTATUS(status));
            printf("PARENT: %s\n", strerror(WEXITSTATUS(status)));
        }
        if (WIFSIGNALED(status))
        {
            printf("PARENT: child terminated with stauts %d\n", WTERMSIG(status));
        }
    }
    return 0;
}


