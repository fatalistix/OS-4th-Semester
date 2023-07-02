#include "./linearmalloc.h"

#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

#define DEBUG



#ifdef DEBUG
    #include <stdio.h>
#endif

typedef struct Data_t 
{
    struct Data_t * prevData;
    struct Data_t * nextData;
    size_t isFree;
} Data;


void * linearmalloc(size_t size)
{
    const size_t mappedRegionSize = getpagesize() * 1000;
    static void * mappedRegionStart = NULL;
    if (mappedRegionStart == NULL)
    {
        mappedRegionStart = mmap(NULL, mappedRegionSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (mappedRegionStart == MAP_FAILED)
        {
            return NULL;
        }
    }

    void * forReturn = NULL;

    size_t realSize = ((size / sizeof(size_t)) + 
            ((size % 8) ? 1 : 0)) * sizeof(size_t);
    Data * currRegionData = (Data *) mappedRegionStart;
    while ((char *) currRegionData < (char *) mappedRegionStart + mappedRegionSize)
    {
        if (!currRegionData->nextData)
        {
            currRegionData->nextData = (Data *) ((char *) currRegionData + 
                    sizeof(Data) + realSize);
            if ((void *) currRegionData == mappedRegionStart)
            {
                currRegionData->prevData = (Data *) mappedRegionStart;
            }
            
            currRegionData->isFree = 0;
            
            Data * nextRegionData = currRegionData->nextData;
            nextRegionData->prevData = currRegionData;
            nextRegionData->nextData = NULL;
            nextRegionData->isFree = 1;
            forReturn = (void *) (currRegionData + 1);
            break;
        }

        if (currRegionData->isFree && realSize + sizeof(Data) < 
                (size_t) ((char *) currRegionData->nextData - 
                    (char *) currRegionData))
        {
            Data * nextNextRegionData = currRegionData->nextData;
            currRegionData->nextData = (Data *) ((char *) currRegionData + 
                    sizeof(Data) + realSize);
            currRegionData->isFree = 0;
            Data * nextRegionData = currRegionData->nextData;
            nextRegionData->prevData = currRegionData;
            nextRegionData->nextData = nextNextRegionData;
            nextRegionData->isFree = 1;
            nextNextRegionData->prevData = nextRegionData;
            forReturn = (void *) (currRegionData + 1);
            break;
        }

        if (currRegionData->isFree && realSize < 
                (size_t) ((char *) currRegionData->nextData - 
                    (char *) currRegionData))
        {
            currRegionData->isFree = 0;
            forReturn = (void *) (currRegionData + 1);
            break;
        }

        currRegionData = currRegionData->nextData;
    }

    if (forReturn == NULL)
    {
        errno = ENOMEM;
    }
    return forReturn;
}


// void * linearmalloc(size_t size) 
// {
//     const size_t mappedRegionSize = getpagesize() * 1000;
//     static void * mappedRegionStart = NULL;
//     if (mappedRegionStart == NULL)
//     {
//         mappedRegionStart = mmap(NULL, mappedRegionSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//     }
//     
//     void * temp = mappedRegionStart;
//     size_t realSize = ((size / sizeof(size_t)) + ((size % 8 == 0) ? 0 : 1)) * sizeof(size_t);
// 
//     void * nextDataPointer = *((void **) temp + 1);
//     void * prevDataPointer = *((void **) temp);
//     size_t isFree = *((size_t *) temp + 2);
// 
//     void * forReturn = NULL;
// 
//     while ((char *) temp < (char *) mappedRegionStart + mappedRegionSize)
//     {
//         if (!nextDataPointer)
//         {
//             nextDataPointer = (void *)((char *) temp + sizeof(void *) + sizeof(size_t) + realSize + sizeof(void *));
//             prevDataPointer = temp;
//             isFree = 0;
//             *((void **) temp) = nextDataPointer;
//             *((size_t *) temp + 1) = isFree;
//             forReturn = (void *) ((char *) temp + sizeof(void *) + sizeof(size_t));
//             temp = nextDataPointer;
//             *((void **) temp - 1) = prevDataPointer;
//             break;
//         }
// 
//         if (isFree && size < (size_t) ((char *) nextDataPointer - (char *) &nextDataPointer - 2 * sizeof(void *) - sizeof(size_t)))
//         {
//             *((size_t *) temp + 1) = 0;
//             forReturn = (void *) ((char *) temp + sizeof(void *) + sizeof(size_t));
//             break;
//         }
// 
// 
// 
//         temp = nextDataPointer;
//         nextDataPointer = *((void **) temp);
//         prevDataPointer = *((void **) temp - 1);
//         isFree          = *((size_t *) temp + 1);
//     }
//     
//     if (forReturn == NULL)
//     {
//         errno = ENOMEM;
//     }
// 
//     return forReturn;
// }
// 
// size_t isDataFree(void * dataPointer)
// {
//     return *((size_t *) dataPointer + 1);
// }
// 
// void * getNextDataPointer(void * blockPointer)
// {
//     return *((void **) blockPointer);
// }


void linearfree(void * pointer)
{
    Data * currRegionData = (Data *) pointer - 1;
    Data * nextRegionData = currRegionData->nextData;
    Data * prevRegionData = currRegionData->prevData;

    if (currRegionData != nextRegionData->prevData)
    {
#ifdef DEBUG
        printf("INVALID POINTER\n");
#endif
        return;
    }
    if (currRegionData->isFree)
    {
#ifdef DEBUG
        printf("DOUBLE FREE OR CORRUPTION\n");
#endif
        return;
    }

    if (prevRegionData->isFree)
    {
        printf("HERE!!!\n");
        prevRegionData->nextData = currRegionData->nextData;
        nextRegionData->prevData = currRegionData->prevData;
        currRegionData = prevRegionData;
    }

    if (nextRegionData->isFree)
    {
        currRegionData->nextData = nextRegionData->nextData;
        Data * nextNextRegionData = (Data *) nextRegionData->nextData;
        nextNextRegionData->prevData = nextRegionData->prevData;
    }

    currRegionData->isFree = 1;
}

// void linearfree(void * pointer)
// {
//     void * nextDataPointer = *((void **) pointer - 2);
//     void * checkPrevDataPointer = *((void **) nextDataPointer - 1);
//     void * prevDataPointer = *((void **) pointer - 3);
//     if (checkPrevDataPointer != (void *) ((void **) pointer - 2))
//     {
// #ifdef DEBUG
//         printf("INVALID POINTER\n");
// #endif
//         return;
//     }
// 
//     size_t isFree = *((size_t *) pointer - 1);
//     if (isFree)
//     {
// #ifdef DEBUG
//         printf("DOUBLE FREE OR CORRUPTION\n");
// #endif
//         return;
//     }
// 
//     *((size_t *) pointer - 1) = 1;
// 
//     if (isDataFree(prevDataPointer))
//     {
//         *((void **) prevDataPointer) = nextDataPointer;
//         checkPrevDataPointer = prevDataPointer;
//         nextDataPointer = *((void **) prevDataPointer);
//         prevDataPointer = *((void **) prevDataPointer - 1);
//     }
// 
//     if (isDataFree(nextDataPointer))
//     {
//         *((void **) nextDataPoiner) = 
//     }
//     
// }

