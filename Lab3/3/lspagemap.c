#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef unsigned long long ull;

ull getPFN(ull pageData) {
     return (pageData & 0x7fffffffffffff);
}

int isSoftDirty(ull pageData) {
    return (int) (pageData >> 55) & 1;
}

int isExclusivelyMapped(ull pageData) {
    return (int) (pageData >> 56) & 1;
}

int isFilePaged(ull pageData) {
    return (int) (pageData >> 61) & 1;
}

int isSwapped(ull pageData) {
    return (int) (pageData >> 62) & 1;
}

int isPresent(ull pageData) {
    return (int) (pageData >> 63) & 1;
}


int printPageMaps(FILE * pageMapsFile, void * startPageAddr, void * endPageAddr, char * name)
{
    int pageSize = getpagesize();

    ull start = (ull) startPageAddr / pageSize;
    ull end   = (ull) endPageAddr   / pageSize;

    if (fseek(pageMapsFile, start * 8, SEEK_SET))
    {
        return errno;
    }

    for (ull i = start; i < end; ++i)
    {
        ull data;
        if (fread(&data, 8, 1, pageMapsFile) != 1)
        {
            if (ferror(pageMapsFile))
            {
                return errno = EINVAL;
            }
        }

        printf("ADDR=%p : PFN=%p : S-D=%d : EXCL-MAP=%d : FIL-PAG=%d : SWAP=%d : PRESENT=%d : NAME=%s\n",
               (char *) startPageAddr + (pageSize * (i - start)), (void *) getPFN(data), isSoftDirty(data), isExclusivelyMapped(data),
               isFilePaged(data), isSwapped(data), isPresent(data), name);
    }

    return 0;
}


int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "%s: missing argument \'PID\'\n", argv[0]);
        return 1;
    }

    printf("lspagemap PID = %d\n", getpid());

    char pageMapsPath[256] = "/proc/";
    strcat(pageMapsPath, argv[1]);
    strcat(pageMapsPath, "/pagemap");

    FILE * pageMapsFile = fopen(pageMapsPath, "r");
    if (pageMapsFile == NULL)
    {
        char error[256];
        sprintf(error, "%s: cannot open PID's pagemap", argv[0]);
        perror(error);
        return errno;
    }



    char mapsPath[256] = "/proc/";
    strcat(mapsPath, argv[1]);
    strcat(mapsPath, "/maps");
    
    FILE * mapsFile = fopen(mapsPath, "r");
    if (mapsFile == NULL)
    {
        char error[256];
        sprintf(error, "%s: cannot open PID's maps", argv[0]);
        perror(error);
        int errorNum = errno;
        fclose(pageMapsFile);
        return errorNum;
    }

    
    char mapsBuf[BUFSIZ];

    while (fgets(mapsBuf, BUFSIZ, mapsFile))
    {
        void * mapStart;
        void * mapEnd;
        char mapPerm[4 + 1];
        char mapOffset[8 + 1];
        int mapDevMajor;
        int mapDevMinor;
        int mapInode;
        char mapName[PATH_MAX] = "";
        if (sscanf(mapsBuf, "%p-%p %s %s %d:%d %d %s", &mapStart, &mapEnd, mapPerm, mapOffset, &mapDevMajor, &mapDevMinor, &mapInode, mapName)) {
            // printf("%p-%p %s %s %d:%d %d %s\n", mapStart, mapEnd, mapPerm, mapOffset, mapDevMajor, mapDevMinor, mapInode, mapName);
            if (printPageMaps(pageMapsFile, mapStart, mapEnd, mapName))
            {
                char error[255];
                sprintf(error, "%s: error reading pagemaps", argv[0]);
                perror(error);
                int errorNum = errno;
                fclose(pageMapsFile);
                fclose(mapsFile);
                return errorNum;
            }
        } 
    }
    
    if (errno) {
        char error[255];
        sprintf(error, "%s: error reading maps", argv[0]);
        perror(error);
        int errorNum = errno;
        fclose(pageMapsFile);
        fclose(mapsFile);
        return errorNum;
    }

    if (fclose(mapsFile) || fclose(pageMapsFile))
    {
        char error[256];
        sprintf(error, "%s: error closing files", argv[0]);
        perror(error);
        return errno;
    }

    // int buf;
    // scanf("%d", &buf);

    return 0;
}
