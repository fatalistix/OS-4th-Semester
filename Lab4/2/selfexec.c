#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    printf("PID = %d\n", getpid());
    sleep(10);
    if (execv(argv[0], argv) == -1)
    {
        char error[256];
        sprintf(error, "%s: error calling exec", argv[0]);
        perror(error);
        return errno;
    }
    printf("Hello, World!\n");
    return 0;
}
