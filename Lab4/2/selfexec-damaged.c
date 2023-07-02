#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    printf("PID = %d\n", getpid());
    sleep(1);
    if (execv("fail", argv) == -1)
    {
        char error[256];
        sprintf(error, "%s: error calling exec", argv[0]);
        perror(error);
    }
    printf("Hello, World!\n");
    return 0;
}
