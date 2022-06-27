#include "mymalloc.h"
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[])
{
    time_t start;
    time_t end;
    int choice = 0;

    void *ptr;
    void* storage[20000];
    memset(storage,0,20000);
    int i = 0;

    int temp = 0;
    int lockcount = 0;
    int throughtput = 0;
    time(&start);

//first round
    myinit(0,myheap);
    while(difftime(time(&end), start) < 2)
    {
        if(difftime(time(&end),start) == 1)
        {
            printf("best fit throughput:%d\n",throughtput);
            utilization();
            mycleanup();
            break;
        }

        choice = rand()%2 + 1;
        switch(choice)
        {
            case 1: //malloc
                ptr = malloc(rand()%256 + 40);
                if(ptr != NULL)
                {
                    storage[i] = ptr;
                    i++;
                }
                throughtput++;
            case 2: //free

                temp = 0;

                while(storage[temp] == 0 && lockcount < i)
                {
                    int temp = rand()%i;
                    lockcount++;
                }
                if(storage[temp] != 0)
                {
                    free(storage[temp]);
                    storage[temp] = 0;
                } 
                lockcount = 0;
                throughtput++;

        }

    }
//second round
    time(&start);
    throughtput = 0;
    myinit(1,myheap);
    while(difftime(time(&end), start) < 2)
    {
        if(difftime(time(&end),start) == 1)
        {
            printf("first fit throughput:%d\n",throughtput);
            utilization();
            mycleanup();
            break;
        }

        choice = rand()%2 + 1;
        switch(choice)
        {
            case 1: //malloc
                ptr = malloc(rand()%256 + 40);
                if(ptr != NULL)
                {
                    storage[i] = ptr;
                    i++;
                }
                throughtput++;
            case 2: //free

                temp = 0;

                while(storage[temp] == 0 && lockcount < i)
                {
                    int temp = rand()%i;
                    lockcount++;
                }
                if(storage[temp] != 0)
                {
                    free(storage[temp]);
                    storage[temp] = 0;
                } 
                lockcount = 0;
                throughtput++;

        }

    }
//third round
    time(&start);
    throughtput = 0;
    myinit(2,myheap);
    while(difftime(time(&end), start) < 2)
    {
        if(difftime(time(&end),start) == 1)
        {
            printf("Next fit throughput:%d\n",throughtput);
            utilization();
            mycleanup();
            break;
        }

        choice = rand()%2 + 1;
        switch(choice)
        {
            case 1: //malloc
                ptr = malloc(rand()%256 + 40);
                if(ptr != NULL)
                {
                    storage[i] = ptr;
                    i++;
                }
                throughtput++;
            case 2: //free

                temp = 0;

                while(storage[temp] == 0 && lockcount < i)
                {
                    int temp = rand()%i;
                    lockcount++;
                }
                if(storage[temp] != 0)
                {
                    free(storage[temp]);
                    storage[temp] = 0;
                } 
                lockcount = 0;
                throughtput++;

        }

    }
    return 0;


}