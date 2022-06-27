
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_MALLOC_SIZE 1048576



struct block
{
    struct block *next;
    struct block *prev;
    size_t actual_size;
    size_t size;
    void *addr;
    uint8_t open;
};
typedef struct block block;

struct heap
{
    char *front;
};
typedef struct heap heap;

char myheap[MAX_MALLOC_SIZE];
static uint8_t initalized = 0;
int fit_method;
struct block *freelist;




char* myinit(int allocAlg, char heap[MAX_MALLOC_SIZE]);

void mycleanup();

void utilization();

#define malloc( x ) mymalloc( x )
void* mymalloc(size_t target_size);

#define free( x ) myfree( x )
void myfree(void *ptr);

#define realloc( x ) myrealloc( x, x)
void* myrealloc(void *ptr, size_t target_size);


