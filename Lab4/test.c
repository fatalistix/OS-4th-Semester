#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    printf("PARENT: PID = %d\n", getpid());
    int x = 10;
    int childPid = fork();
    if (childPid == 0)
    {
        printf("CHILD: PID = %d\n", getpid());
        printf("CHILD:  x = %d\n", x);
        sleep(15);
        printf("CHILD:  x = %d after modifications\n", x);
        sleep(15);
    }
    else 
    {
        sleep(10);
        printf("PARENT: x = %d\n", x);
        x = 20;
        printf("PARENT: x = %d after modifications\n", x);
        sleep(10);
    }
    return 0;
}
