#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <fcntl.h>








int main(int argc, char * argv[]) 
{ 
    if (argc < 2)
    {
        fprintf(stderr, "%s: missing operand (PID)\n", argv[0]);
        return 1;
    }


    char * pagemaps = (char *) malloc(sizeof(char) * PAGEMAPS_BUFFER_SIZE); 
    if (pagemaps == NULL) 
    { 
        char error[256] = ""; 
        perror(strcat(strcat(error, argv[0]), ": error allocating memory")); 
        return errno; 
    }

    char pagemapPath[256] = "/proc/";
    strcat(pagemapPath, argv[1]);
    strcat(pagemapPath, "/pagemap");

    FILE * pmFile = fopen(pagemapPath, "r");
    if (pmFile == NULL)
    {
        char error[256] = "";
        perror(strcat(strcat(error, argv[0]), ": cannot open pagemap file"));
        free(pagemaps);
        return errno;
    }
    
    while (fread(pagemaps, 1, PAGEMAPS_BUFFER_SIZE, pmFile) == PAGEMAPS_BUFFER_SIZE)
    {
        printf("1\n");
    }

    if (fclose(pmFile) != 0)
    {
        char error[256] = "";
        strcat(error, argv[0]);
        strcat(error, ": error closing pagemap file");
        perror(error);
        free(pagemaps);
        return errno;
    }
    free(pagemaps);
    return 0;
}
