#include "mymalloc.h"
#include <stdio.h>


char* myinit(int allocAlg, char myheap[MAX_MALLOC_SIZE])
{
    freelist = (struct block *)myheap;
    freelist->next = NULL;
    freelist->prev = NULL;
    freelist->addr = myheap + sizeof(struct block);
    freelist->size = MAX_MALLOC_SIZE - sizeof(struct block);
    freelist->actual_size = 0;
    freelist->open = 1;

    //memcpy(myheap, &freelist, sizeof(struct block));

    //freelist = (block *)(myheap + sizeof(struct block));
    //printf("initalized --- current freelist's addr is: %p\n", freelist->addr);
    initalized = 1;
    fit_method = allocAlg;
    return myheap;
}

void* mymalloc(size_t target_size)
{
    if(initalized == 0)
    {
        myinit(0, myheap);
        initalized = 1;
    }

    if(target_size > MAX_MALLOC_SIZE)
    {
        printf("Too large for this 1MB heap\n");
        return NULL;
    }

    //block *front = (block *)myheap;

//find the best free block - best fit
    block *best_choice = NULL;
    struct block *front = freelist;
    if(fit_method == 0)
    {
        while(front!=NULL)
        {
            if(front->open == 1 && front->size >= target_size)
            {
                if(!best_choice)
                {
                    best_choice = front;
                } else {
                    if(front->size < best_choice->size)
                    {
                        best_choice = front;
                    }
                }
            }
            front = front->next;
        }
    }else // first fit
    {
        while(front!=NULL && best_choice == NULL)
        {
            if(front->open == 1 && front->size >= target_size)
            {
                if(!best_choice)
                {
                    best_choice = front;
                } else {
                    if(front->size < best_choice->size)
                    {
                        best_choice = front;
                    }
                }
            }
            front = front->next;
        }
    }




// if no block
    if(!best_choice)
    {
        //printf("no open blocks\n");
        return NULL;
    }
// allocate

    size_t reserve_size = 0;
    if(target_size % 8 != 0)
    {
        size_t temp = target_size % 8;
        reserve_size = target_size - temp + 8;
    }else{
        reserve_size = target_size;
    }

    if(best_choice->size >= reserve_size) //at least can successfully allocate this command
    {
        if(best_choice->size >= (reserve_size + sizeof(struct block))) // if there are enough space to start next free block
        {
                
                //bestchoice <------> new <-------> bestchoice_next
                struct block new;
                new.next = best_choice->next;
                new.prev = best_choice;
                new.addr = (char *)best_choice + reserve_size + sizeof(struct block);
                new.size = best_choice->size - reserve_size - sizeof(struct block);
                new.open = 1;

                memcpy((char *)best_choice + reserve_size, &new, sizeof(struct block));

                best_choice->next = (char *)best_choice + reserve_size;
                best_choice->open = 0;
                best_choice->size = reserve_size;
                best_choice->actual_size = target_size;

                //test printing info
                //printf("successfully allocate %ld byte memory --- actual requirement is: %ld\n", reserve_size, target_size);

                //if current block is the freelist head, change freelist
                if(best_choice->addr == freelist->addr)
                {
                    freelist = (struct block *)(new.addr - sizeof(struct block));
                }

                return best_choice->addr;

        }else{
            best_choice->next = NULL;
            best_choice->open = 0;
            best_choice->size = reserve_size;
            best_choice->actual_size = target_size;

            //printf("successfully allocate %ld byte memory\n", target_size);
            //printf("However, there is no more space or free blocks in the heap\n");
            return best_choice->addr;

        }
    } else {
        //printf("no large enough free blocks for requirement\n");
        return NULL;
    }

   // printf("after the process, for some reason, no memory allocated successfully\n");
    return NULL;
}


void myfree(void *ptr)
{
    if(initalized == 0)
    {
        printf("no heap has been initialized, please allocate some memory first\n");
        return;
    }
    if(ptr == NULL)
    {
        printf("ptr is null, does nothing\n");
        return;
    }

    struct block *front = (struct block *)myheap;
    //if first few blocks has been freed after initialed
    if(freelist != front)
    {
        while(front != NULL && front->open == 1)
        {
            front = (struct block *)(front + front->size);
        }
        //first step check
        if(front == NULL)
        {
            //printf("not allocated block found\n");
            return;
        }

        //after find the first allocated block
        while (front->prev != NULL)
        {
            front = front->prev;
        }
        //second pass finished, the front will be the head to the allocated memory chain
    }

 
    while(front != NULL)
    {
        //check double free or trying to free a empty block
        if(front->addr == ptr)
        {
            if(front->open == 1)
            {
                printf("this block has not been allocated or it has been freed already\n");
                return;
            }
            //else, start process
            front->open = 1;
            front->actual_size = 0;
            int change_freelist = 0;
            
            int next_change = 0;
                //merge right block if empty
            if(front->next!=NULL && front->next->open == 1)
            {
                    //check if need to change freelist
                if(front->next->addr == freelist->addr)
                {
                    change_freelist = 1;
                }
                next_change = 1;
                front->size += front->next->size;
                front->next = front->next->next;
                if(front->next != NULL)
                {
                    front->next->prev = front;
                }
                    //change freelist
                if(change_freelist == 1)
                {
                    freelist = (struct block *)(front->addr - sizeof(struct block));
                    change_freelist = 0;
                }
                
            }
            int prev_change = 0;
                //merge left block if empty
            if(front->prev != NULL && front->prev->open == 1)
            {
                    //check if need to change freelist
                if(front->prev->addr == freelist->addr)
                {
                    change_freelist = 1;
                }

                prev_change = 1;
                front->prev->size += front->size;
                if(front->next != NULL)
                {
                    front->next->prev = front->prev;
                }
                front->prev->next = front->next;

                    //change freelist
                if(change_freelist == 1)
                {
                    freelist = (struct block *)(front->prev->addr - sizeof(struct block));
                    change_freelist = 0;
                }

            }
            //bind this block to free list


                block *temp = freelist;
                while(temp->next != NULL)
                {
                    temp = temp->next;
                }
                if(prev_change == 1)
                {
                    if(front->prev != freelist)
                    {
                        temp->next = front->prev;
                    }
                    
                }else if (next_change == 1){
                    if(front != freelist)
                    {
                        temp->next = front;
                    }        
                }else if (prev_change == 0 && next_change == 0)
                {


                    temp->next = front;
                    
                    if(front->prev!=NULL)
                    {
                        if(front->next != NULL)
                        {
                            front->prev->next = front->next;
                            front->next->prev = front->prev;
                        }
                    }
                    
                    

                    front->next = NULL;
                    front->prev = temp;

                    //test area
                    if(freelist->next != NULL)
                    {
                        printf("first bind block successed\n");
                    }
                }
            

            return;

        }

        front = front->next;      
    }

    printf("nothing happened after process\n");
    printf("possible reason: double free or try to free an unallocated memory\n");
}

void mycleanup()
{
    memset(myheap, 0, MAX_MALLOC_SIZE);
    initalized = 0;
}

void utilization()
{
    if(initalized == 0)
    {
        //printf("no memory has been allocated\n");
        return;
    }
    struct block *front = (struct block *)myheap;

    while(front!=NULL && front->open == 1)
    {
        front = (struct block *)(front + front->size);
    }

    if(front == NULL)
    {
        //printf("no allocated memory\n");
        return;
    }else
    {
        while(front->prev!= NULL)
        {
            front = front->prev;
        }
    }

    //calculate
    int used = 0;
    int allocated = 0;
    float result = 1.00;
    while(front!=NULL && front->open != 1)
    {
        used += (int)front->actual_size;
        allocated += (int)front->size;
        front = front->next;
    }
    //printf("used: %d --- allocated:%d\n", used, allocated);
    result = 100*((float)used / (float)allocated);

    printf("utilization rate: %.2f\n", result);
    return;
}

void* myrealloc(void *ptr, size_t target_size)
{
    if(ptr == NULL && target_size == 0)
    {
        printf("ptr is null and target size = 0, does nothing, return\n");
        return NULL;
    }
    if(ptr == NULL)
    {
        void* result = mymalloc(target_size);
        return result;
    }
    if(target_size == 0)
    {
        myfree(ptr);
        return NULL;
    }

    //regular case
    myfree(ptr);
    void * result = mymalloc(target_size);
    if(result == NULL)
    {
        printf("for some reason, reallocation can't be done\n");
        return NULL;
    }
    return result;

}