#include <errno.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>




bool strStartsWith(const char * str, const char * prefix) 
{
    int i = 0;
    while (str[i] != 0 && prefix[i] != 0 && str[i] == prefix[i]) { ++i; }
    if (prefix[i] == 0) { return true; }
    return false;
}

char * pathToName(const char * path, char * name) 
{
    char * pathNamePointer = strrchr(path, '/');
    ++pathNamePointer;
    while (*pathNamePointer != 0) 
    {
        *name = *pathNamePointer;
        ++name; ++pathNamePointer;
    }
    return name;
}

char * strReverse(const char * src, char * res)
{
    int j = 0;
    for (int i = strlen(src) - 1; i >= 0; --i) 
    {
        res[j++] = src[i];
    }
    res[j] = '\0';
    return res;
}



int main(int argc, char * argv[]) 
{   
    if (argc < 2) 
    {
        fprintf(stderr, "cpdirr: missing src directory operand\n");
        return 1;
    }
    if (argc < 3)
    {
        fprintf(stderr, "cpdirr: missing dest directory operand\n");
        return 1;
    }


    char srcDirPathBuf[PATH_MAX]; 
    char destDirPathBuf[PATH_MAX];
    char * srcDirPath = realpath(argv[1], srcDirPathBuf);
    char * destDirPath = realpath(argv[2], destDirPathBuf);


    if (srcDirPath == NULL) 
    {
        perror("cpdirr: resolving path error");
        return errno;
    }
    if (destDirPath == NULL) 
    {
        perror("cpdirr: resolving path error");
        return errno;
    }



    if (strStartsWith(destDirPath, srcDirPath))
    {
        fprintf(stderr, "cpdirr: dest directory can't be src directory or it's subdirectory\n");
        return EXIT_FAILURE;
    }

    
    char srcDirName[255];
    pathToName(srcDirPath, srcDirName);
    char reversedSrcDirName[255];
    strReverse(srcDirName, reversedSrcDirName);


    struct stat statRes;
    if (stat(srcDirPath, &statRes) != 0)
    {
        perror("cpdirr: cannot stat src dir");
        return errno;
    }

    if (!S_ISDIR(statRes.st_mode))
    {
        fprintf(stderr, "cpdirr: \'%s\' is not a directory\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (mkdir(strcat(strcat(destDirPath, "/"), reversedSrcDirName), statRes.st_mode) != 0) 
    {
        char error[255] = "cpdirr: cannot mkdir \'";
        perror(strcat(strcat(error, reversedSrcDirName), "\'"));
        return errno;
    }

    return 0;
}


