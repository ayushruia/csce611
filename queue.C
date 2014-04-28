
#include "queue.H"


Queue::Queue(unsigned int _block_size, unsigned int _no_blocks)
{
    head = 0;
    tail = 0;
    block_size = _block_size;
    no_blocks = _no_blocks;
    max_size = block_size*no_blocks;
    start_add = new char[max_size];
    memset(start_add,0,max_size);
}




int Queue::enqueue(char *enqthread)
{
    if(((tail + 1)%(no_blocks)) == head)
        return -1;
    char * start_loc;
    start_loc = start_add + block_size*tail;
    for(int i=0; i<block_size; i++)
        *(start_loc + i) = *(enqthread + i);
    tail = (tail + 1)%(no_blocks);
    return 0;
}

int Queue::dequeue(char *deqthread)
{
    if(head == tail)
    {
        memset(deqthread,0,(block_size));
        return -1;
    }
    char * start_loc;
    start_loc = start_add + (block_size)*(head);
    for(int i=0; i<block_size; i++)
        *(deqthread + i) = *(start_loc + i);
    head = (head + 1)%(no_blocks);
    return 0;
}

int Queue::pushqueue(char *enqthread)
{
    if(((tail + 1)%(no_blocks)) == head)
        return -1;
    char * start_loc;
    start_loc = start_add + (block_size)*((head + no_blocks -1)%(no_blocks));
    for(int i=0; i<block_size; i++)
        *(start_loc + i) = *(enqthread + i);
    head = (head + no_blocks - 1)%(no_blocks);
    return 0;
}

int Queue::no_elements()
{
    return (tail+no_blocks-head)%no_blocks;
}

BOOLEAN Queue::is_full()
{
    if(no_elements() == no_blocks-1)
        return TRUE;
    return FALSE;
}

