#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void mySigsegvHandler(int signum)
{
    printf("BROKEN\n");
    exit(SIGSEGV);
}

int main(int argc, char * argv[])
{
    printf("<==> PID=%d <==>\n", getpid());
    sleep(10);

    int sizeToMap = getpagesize() * 10;

    void * mappedRegion = mmap(NULL, sizeToMap, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mappedRegion == MAP_FAILED)
    {
        char error[256];
        sprintf(error, "%s: error mapping region", argv[0]);
        perror(error);
        return errno;
    }
    
    printf("<==> NEW REGION MAPPED <==>\n");
    sleep(5);

    if (mprotect(mappedRegion, sizeToMap, PROT_READ | PROT_EXEC) == -1)
    {
        char error[256];
        sprintf(error, "%s: error changing mapped region permissions", argv[0]);
        perror(error);
        munmap(mappedRegion,sizeToMap);
        return errno;
    }

    printf("<==> PART OF REGION PERMISSIONS CHANGED <==>\n");

    sleep(5);

    int canRead = 0;

    if (argc > 1)
    {
        canRead = atoi(argv[1]);
    }

    printf("<==> CAN READ = %d <==>\n", canRead);
    
    
    signal(SIGSEGV, mySigsegvHandler);

    if (canRead == 1)
    {
        if (mprotect(mappedRegion, sizeToMap, PROT_READ) == -1)
        {
            char error[256];
            sprintf(error, "%s: error changing mapped region permissions", argv[0]);
            perror(error);
            munmap(mappedRegion,sizeToMap);
            return errno;
        }

        printf("<==> PERM CHANGED TO READ AND NOT TO WRITE <==>\n");
        printf("<==> WRITING \'40\' TO %p <==>\n", mappedRegion + 10);
        *((char *) mappedRegion + 10) = 40;
        printf("<==> WRITED <==>\n");
    }
    else if (canRead == 0)
    {
        if (mprotect(mappedRegion, sizeToMap, PROT_NONE) == -1)
        {
            char error[256];
            sprintf(error, "%s: error changing mapped region permissions", argv[0]);
            perror(error);
            munmap(mappedRegion,sizeToMap);
            return errno;
        }
       
        printf("<==> PERM CHANGED TO NONE <==>\n");
        char buf = 100;
        // *((char *) mappedRegion + 10) = 48;
        // printf("<==> WRITED DATA TO REGION <==>\n");
        // printf("<==> READING DARA FROM %p <==>\n", mappedRegion + 10);
        buf = *((char *) mappedRegion + 10);
        printf("<==> BUFFERED CHAR VAR = %d <==>\n", buf);
    }

    sleep(3);
    
    if (munmap(mappedRegion + getpagesize() * 4, getpagesize() * 3) == -1)
    {
        char error[256];
        sprintf("%s: error unmapping regions", argv[0]);
        perror(error);
        return errno;
    }

    printf("<==> UNMAPPED 4-6 <==>\n");

    sleep(15);

    if (munmap(mappedRegion, sizeToMap) == -1)
    {
        char error[256];
        sprintf("%s: error unmapping regions", argv[0]);
        perror(error);
        return errno;
    }

    printf("<==> UNMAPPED ALL <==>\n");
    sleep(10);

    
    return 0;
}
