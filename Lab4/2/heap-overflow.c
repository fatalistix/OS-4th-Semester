#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

//? 128 iterations with malloc(1024) calls brk
//? see mmap with malloc(1024 * 1024)

void freeArrayInRange(void * array[], int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        free(array[i]);
    }
}

int main(int argc, char * argv[])
{
    int mallocBlockSize = 1024 * 1024;
    if (argc > 2)
    {
        mallocBlockSize = atoi(argv[1]);
    }

    printf("<==> PID=%d <==>\n\n\n<==> BLOCK=%d <==>\n", getpid(), mallocBlockSize);
    sleep(10);

    const int size = 1000;
    void * pointersArray[size];

    for (int i = 0; i < size; ++i)
    {
        pointersArray[i] = malloc(mallocBlockSize);
        if (pointersArray[i] == NULL)
        {
            char error[256];
            sprintf(error, "%s: error allocating memory", argv[0]);
            perror(error);
            freeArrayInRange(pointersArray, 0, i);
            return errno;
        }
        printf("%d with %p\n", i, pointersArray[i]);
        usleep(250000);
    }

    freeArrayInRange(pointersArray, 0, size);

    // int sizeToMap = getpagesize() * 10;

    // void * mappedRegion = mmap(NULL, sizeToMap, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS, -1, 0);

    // sleep(10);
    // 
    // if (mprotect(mappedRegion, getpagesize() * 3, PROT_READ | PROT_EXEC) == -1)
    // {
    //     char error[256];
    //     sprintf(error, "%s: error changing mapped region permissions", argv[0]);
    //     perror(error);
    //     munmap(mappedRegion,sizeToMap);
    //     return errno;
    // }
    // 
    // if (argc > 3)
    // {
    // 
    // }

    return 0;
}
