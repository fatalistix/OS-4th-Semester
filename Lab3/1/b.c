 #include <dirent.h>
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


char * strDeleteTillR(char * str, char c)
{
    char * buf = strrchr(str, c);
    ++buf;
    *buf = 0;
    return str;
}

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "cpdirwfr: missing src directory operand\n");
        return EXIT_FAILURE;
    }
    if (argc < 3)
    {
        fprintf(stderr, "cpdirwfr: missing dest directory operand\n");
        return EXIT_FAILURE;
    }

    char srcDirPathBuf[PATH_MAX]; 
    char destDirPathBuf[PATH_MAX];
    char * srcDirPath = realpath(argv[1], srcDirPathBuf);
    char * destDirPath = realpath(argv[2], destDirPathBuf);



    if (srcDirPath == NULL) 
    {
        perror("cpdirwfr: resolving path error");
        return errno;
    }
    if (destDirPath == NULL) 
    {
        perror("cpdirwfr: resolving path error");
        return errno;
    }

    // if (strStartsWith(destDirPath, srcDirPath))
    // {
    //     fprintf(stderr, "cpdirwfr: dest directory can't be src directory or it's subdirectory");
    //     return EXIT_FAILURE;
    // }

    char srcDirName[255];
    pathToName(srcDirPath, srcDirName);
    char reversedSrcDirName[255];
    strReverse(srcDirName, reversedSrcDirName);

    DIR * srcDir = opendir(srcDirPath);
    if (srcDir == NULL) 
    {
        char error[PATH_MAX + 255] = "cpdirwfr: cannot open dir \'";
        perror(strcat(strcat(error, argv[1]), "\'"));
        return errno;
    }

    struct stat statRes;
    if (stat(srcDirPath, &statRes) != 0)
    {
        closedir(srcDir);
        perror("cpdirwfr: cannot stat src dir");
        return errno;
    }


    
    //! destDirPath changes
    if (mkdir(strcat(strcat(destDirPath, "/"), reversedSrcDirName), statRes.st_mode) != 0) 
    {
        closedir(srcDir);
        char error[255] = "cpdirr: cannot mkdir \'";
        perror(strcat(strcat(error, reversedSrcDirName), "\'"));
        return errno;
    }





    struct dirent * child;
    strcat(srcDirPath, "/");
    strcat(destDirPath, "/");

    while ((child = readdir(srcDir)) != NULL)
    {
        strDeleteTillR(srcDirPath, '/');
        strcat(srcDirPath, child->d_name);

        struct stat statFileRes;
        if (stat(srcDirPath, &statFileRes) != 0)
        {
            closedir(srcDir);
            // printf("%s\n", srcDirPath);
            perror("cpdirwfr: cannot stat src dir's file");
            return errno;
        }

        if (S_ISREG(statFileRes.st_mode))
        {
            strDeleteTillR(destDirPath, '/');
            char reversedName[255];
            strReverse(child->d_name, reversedName);
            strcat(destDirPath, reversedName);
            printf("%s\n", destDirPath);
            FILE * regDestFile = fopen(destDirPath, "wb");
            if (regDestFile == NULL)
            {
                closedir(srcDir);
                char error[255] = "cpdirwfr: cannot open file \'";
                perror(strcat(strcat(error, reversedName), "\'"));
                return errno;
            }

            FILE * regSrcFile = fopen(srcDirPath, "rb");
            if (regSrcFile == NULL)
            {
                closedir(srcDir);
                fclose(regDestFile);
                char error[255] = "cpdirwfr: cannot open file \'";
                perror(strcat(strcat(error, reversedName), "\'"));
                return errno;
            }

            if (fseek(regSrcFile, 0, SEEK_END) != 0)
            {
                closedir(srcDir);
                fclose(regDestFile);
                fclose(regSrcFile);
                perror("cpdirwfr: cannot seek to end of file");
                return errno;
            }

            int pos = ftell(regSrcFile);
            if (pos == -1)
            {
                closedir(srcDir);
                fclose(regDestFile);
                fclose(regSrcFile);
                perror("cpdirwfr: error obtaining file pos inidicator");
                return errno;
            }

            while (pos--)
            {
                if (fseek(regSrcFile, pos, SEEK_SET) != 0)
                {
                    closedir(srcDir);
                    fclose(regDestFile);
                    fclose(regSrcFile);
                    perror("cpdirwfr: cannot seek in file");
                    return errno;
                }

                fputc(fgetc(regSrcFile), regDestFile);
            }
            
            // printf("opened and created %s\n", reversedName);
            fclose(regDestFile);
            fclose(regSrcFile);
        }
    }


    closedir(srcDir);
    return 0;
}