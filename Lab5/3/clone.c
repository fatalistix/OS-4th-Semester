#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>


static const int STACK_SIZE = 8192;


void recursiveHelloWorld(int index)
{
    if (index < 10)
    {
        char helloWorldStr[] = "Hello, World!\n";
        recursiveHelloWorld(index + 1);
    }
}

int enterPoint(void * arg)
{
    recursiveHelloWorld(0);
    return 0;
}


int errorHandler(char * message, char * programName)
{
    char error[256];
    sprintf(error, "%s: ", programName);
    strcat(error, message);
    perror(error);
    return errno;
}

int main(int argc, char * argv[])
{
    int fd = open("./stack.shared", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
    if (fd == -1)
    {
        return errorHandler("error opening file", argv[0]);
    }

    if (ftruncate(fd, STACK_SIZE)) 
    {
        return errorHandler("error calling ftruncate", argv[0]);
    }

    void * fileStackPointer = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, 0);
    if (fileStackPointer == MAP_FAILED)
    {
        return errorHandler("error calling mmap", argv[0]);
    }

    int childPid = clone(enterPoint, fileStackPointer + STACK_SIZE, CLONE_VM, NULL);
    if (childPid == -1)
    {
        return errorHandler("error calling clone", argv[0]);
    }

    if (waitpid(childPid, NULL, __WCLONE) == -1)
    {
        return errorHandler("error waiting for child", argv[0]);
    }


    if (close(fd) == -1)
    {
        return errorHandler("error closing file", argv[0]);
    }

    return 0;
}
