#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * All functions below returns 0 if SUCCESS and -1 if an error occured.
 * Error value can be checked via errno and/or perror
*/

int createDir     (const char name[], int permissions);             //* (A)
int printDirectory(const char path[]);                              //* (B)
int removeDir     (const char path[]);                              //* (C)
int createFile    (const char name[]);                              //* (D)
int printFile     (const char path[]);                              //* (E)
int removeFile    (const char path[]);                              //* (F) 
int createSymLink (const char filepath[], const char linkpath[]);   //* (G)
int printSymLink  (const char path[]);                              //* (H)
// int printWhereLink(const char path[]);                              //* (I)
// int unlinkSymLink (const char path[]);                              //* (J)
int createLink    (const char filepath[], const char linkpath[]);   //* (K)

int printStats    (const char path[]);                              //* (M)
int changePerm    (const char path[], int permissions);             //* (N)



int createDir(const char name[], int permissions)
{   //? this foo returns -1 if error occured
    return mkdir(name, permissions);
}

int printDirectory(const char path[])
{
    DIR * forPrint = opendir(path);
    if (forPrint == NULL)
    {
        return -1;
    }

    struct dirent * child;

    while ((child = readdir(forPrint)) != NULL)
    {
        printf("%s\n", child->d_name);
    }

    return closedir(forPrint);
}

int removeDir(const char path[]) 
{
    return rmdir(path);
}

int createFile(const char name[])
{
    FILE * f = fopen(name, "w");
    if (f == NULL)
    {
        return -1;
    }
    return fclose(f);
}

int printFile(const char path[])
{
    FILE * file = fopen(path, "r");
    if (file == NULL)
    {
        return -1;
    }
    
    const int readBufferSize = 4096; 
    char readBuffer[4096];
    int readBytes;
    while ((readBytes = fread(readBuffer, 1, readBufferSize, file)) == readBufferSize)
    {
        if (fwrite(readBuffer, 1, readBufferSize, stdout) != readBufferSize)
        {
            int err = errno;
            fclose(file);
            errno = err;
            return -1;
        }
    }
    if (feof(file))
    {
        if (fwrite(readBuffer, 1, readBytes, stdout) != readBytes)
        {
            int err = errno;
            fclose(file);
            errno = err;
            return -1;
        }
    }
    // printf("\n");
    return fclose(file);
}

int removeFile(const char path[])
{
    return remove(path);
}

int createSymLink(const char filepath[], const char linkpath[])
{
    return symlink(filepath, linkpath);
}

int printSymLink(const char path[])
{
    char buffer[PATH_MAX];
    if (readlink(path, buffer, PATH_MAX) == -1)
    {
        return -1;
    }
    printf("%s\n", buffer);
    return 0;
}

// int unlinkSymLink(const char path[])
// {
//     return unlink(path);
// }

int createLink(const char filepath[], const char linkpath[])
{
    return link(filepath, linkpath);
}

int printStats(const char path[])
{
    struct stat statRes;
    if (stat(path, &statRes) != 0)
    {
        return -1;
    }

    printf("Mode = %d; Links num = %lu\n", statRes.st_mode, statRes.st_nlink);
    return 0;
}

int changePerm(const char path[], int permissions)
{
    return chmod(path, permissions);
}



int main(int argc, char * argv[])
{
    char * namePointer = strrchr(argv[0], '/');
    if (namePointer == NULL) { namePointer = argv[0]; }
    else { ++namePointer; }

    int code = 0;

    if (argc < 2)
    {
        fprintf(stderr, "%s: missing operand\n", argv[0]);
        return 1;
    }

    if (strcmp(namePointer, "umkdir") == 0)
    {
        struct stat statProgPerm;
        if (stat(argv[0], &statProgPerm) != 0)
        {
            char error[255] = "";
            strcat(error, argv[0]);
            perror(strcat(error, ": cannot stat executed file"));
            return errno;
        }

        code = createDir(argv[1], statProgPerm.st_mode);
    } 
    
    else if (strcmp(namePointer, "uls") == 0)
    {
        code = printDirectory(argv[1]);
    }

    else if (strcmp(namePointer, "urmdir") == 0)
    {
        code = removeDir(argv[1]);
    }

    else if (strcmp(namePointer, "utouch") == 0)
    {
        code = createFile(argv[1]);
    }

    else if (strcmp(namePointer, "ucat") == 0)
    {
        code = printFile(argv[1]);
    }

    else if (strcmp(namePointer, "urm") == 0)
    {
        code = removeFile(argv[1]);
    }

    else if (strcmp(namePointer, "usymlink") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "%s: missing second operand\n", argv[0]);
            return 1;
        }

        code = createSymLink(argv[1], argv[2]);
    }

    else if (strcmp(namePointer, "ureadlink") == 0)
    {
        code = printSymLink(argv[1]);
    }

    else if (strcmp(namePointer, "usymunlink") == 0 || strcmp(namePointer, "uunlink") == 0)
    {
        code = unlink(argv[1]);
    }

    else if (strcmp(namePointer, "ulink") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "%s: missing second operand\n", argv[0]);
            return 1;
        }

        code = createLink(argv[1], argv[2]);
    }

    else if (strcmp(namePointer, "ustat") == 0)
    {
        code = printStats(argv[1]);
    }

    else if (strcmp(namePointer, "uchmod") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "%s: missing second operand\n", argv[0]);
            return 1;
        }
        char * end;
        code = changePerm(argv[1], strtol(argv[2], &end, 8));
    }

    if (code != 0)
    {
        char error[255];
        perror(strcat(error, argv[0]));
        return errno;
    }


    return 0;
}
